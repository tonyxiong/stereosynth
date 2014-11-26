/* 
 * File:   randsearch.h
 * Author: akaspar
 *
 * Created on November 24, 2014, 9:37 PM
 */

#ifndef RANDSEARCH_H
#define	RANDSEARCH_H

#include "nnf.h"
#include "trypatch.h"
#include "../im/patch.h"
#include "../math/bounds.h"

namespace pm {
    
    template < typename Patch = Patch2ti, typename DistValue = float, int K = 7 >
    struct RandomSearch {
        typedef NearestNeighborField<Patch, DistValue, K> NNF;

        bool operator()(const Point2i &i, bool);

        RandomSearch(NNF *nnf);
    };
    
    template < typename S >
    struct SearchRadius {
        
        SearchRadius() : radius(100), minimum(5), decreaseFactor(2){}
        
        S radius;
        S minimum;
        S decreaseFactor;
        
        static SearchRadius<S> *create(){
            radiuses.push_back(SearchRadius<S>());
            return &radiuses.back();
        }
        
    private:
        static std::vector< SearchRadius<S> > radiuses;
    };
    
#define SEARCH_RADIUS_REGISTER(type) \
    std::vector< SearchRadius<type> > SearchRadius<type>::radiuses;
    
    template < typename S >
    struct DecreasingSearchRadius {
        
        DecreasingSearchRadius() : search(NULL) {}
        explicit DecreasingSearchRadius(SearchRadius *sr) : search(sr) {}
            
        bool operator()(int, bool){
            search.radius = std::max(search->minimum, search->radius / search->decreaseFactor);
            return false;
        }
        
    private:
        SearchRadius<S> *search;
    };
    
    // Implementation for 2d translation patches
    template < typename S, typename DistValue>
    class RandomSearch<BasicPatch<S>, DistValue, 1> {
    public:
        typedef BasicPatch<S> TargetPatch;
        typedef typename BasicPatch<S>::point point;
        typedef typename point::vec vec;
        typedef NearestNeighborField<TargetPatch, DistValue, 1> NNF;
        typedef Bounds<S, 2> bounds;

        bool operator()(const Point2i &i, bool){
            
            // target patch
            const TargetPatch &p = nnf->patch(i);
            
            // search bounds
            const FrameSize &target = nnf->targetSize().shrink(TargetPatch::width());
            bounds b = bounds(vec(0, 0), vec(target.width, target.height)) & bounds(p, search->radius);
            
            // uniformly sample a position for the new patch
            const point &q = uniform(nnf->rng(), b.min, b.max);
            return tryPatch<TargetPatch, DistValue>(nnf, i, TargetPatch(q));
        }

        RandomSearch(NNF *n, const SearchRadius<S> *sr) : nnf(n), search(sr){}
        
    private:
        NNF *nnf;
        const SearchRadius<S> *search;
    };
    
    // Implementation for 2d translation patches and k-NN
    template < typename S, typename DistValue, int K>
    class RandomSearch<BasicPatch<S>, DistValue, K> {
    public:
        typedef BasicPatch<S> TargetPatch;
        typedef typename BasicPatch<S>::point point;
        typedef typename point::vec vec;
        typedef NearestNeighborField<TargetPatch, DistValue, K> NNF;
        typedef Bounds<S, 2> bounds;

        bool operator()(const Point2i &i, bool){
            
            // target patches
            point p[K];
            for(int k = 0; k < K; ++k){
                p[k] = nnf->patch(i, k);
            }
            
            // bounds
            const FrameSize &target = nnf->targetSize().shrink(TargetPatch::width());
            bounds frame(vec(0, 0), vec(target.width, target.height));
            
            // sample in window defined by the current patch and the given radius
            bool success = false;
            for(int k = 0; k < K; ++k){
                bounds b = frame & bounds(p[k], search->radius);
                const point &q = uniform(nnf->rng(), b.min, b.max);
                success |= kTryPatch<K, TargetPatch, DistValue>(nnf, i, TargetPatch(q));
            }
            return success;
        }

        RandomSearch(NNF *n, const SearchRadius<S> *sr) : nnf(n), search(sr){}
        
    private:
        NNF *nnf;
        const SearchRadius<S> *search;
    };
    
}

#endif	/* RANDSEARCH_H */
