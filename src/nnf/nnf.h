/* 
 * File:   nnf.h
 * Author: Alexandre Kaspar <akaspar@mit.edu>
 *
 * Created on November 21, 2014, 11:17 AM
 */

#ifndef NNF_H
#define	NNF_H

#include "distance.h"
#include "field.h"
#include "patch.h"
#include "../sampling/uniform.h"

namespace pm {
    
    typedef unsigned int uint;
    
#ifndef ONLY_K_NNF
#define ONLY_K_NNF 0
#endif

    // nearest neighbor field
    template < typename Patch = Patch2ti, typename DistValue = float, int K = 7 >
    struct NearestNeighborField : Field2D<true> {

        typedef Patch TargetPatch;
        typedef DistValue DistanceValue;
        typedef typename Patch::point point;
        typedef typename point::vec vec;
        
        // --- provide a common RNG instance -----------------------------------
        RNG rng() const;
        // --- provide a target dimension --------------------------------------
        FrameSize targetSize() const;
        // --- provide patch and distance storage ------------------------------
        const TargetPatch &patch(const Point2i &i, int k) const;
        const DistValue &distance(const Point2i &i, int k) const;
        bool store(const Point2i &i, const TargetPatch &p, const DistValue &d);
        // --- provide a distance computation ----------------------------------
        DistanceValue dist(const Point2i &pos, const TargetPatch &q);
        // --- provide a filtering for special cases (auto-nnf) ----------------
        bool filter(const Point2i &pos, const TargetPatch &q) const;
    };
    
    template < typename S, typename DistValue, int K >
    struct NearestNeighborField<BasicIndexedPatch<S>, DistValue, K> : Field2D<true> {

        typedef BasicIndexedPatch<S> TargetPatch;
        typedef DistValue DistanceValue;
        typedef typename BasicIndexedPatch<S>::point point;
        typedef typename point::vec vec;
        
        // --- provide a common RNG instance -----------------------------------
        RNG rng() const;
        // --- provide the target space dimensions -----------------------------
        size_t targetCount() const;
        FrameSize targetSize(size_t n) const;
        // --- provide patch and distance storage ------------------------------
        const TargetPatch &patch(const Point2i &i, int k) const;
        const DistValue &distance(const Point2i &i, int k) const;
        bool store(const Point2i &i, const TargetPatch &p, const DistValue &d);
        // --- provide a distance computation ----------------------------------
        DistanceValue dist(const Point2i &pos, const TargetPatch &q);
    };
    
#if !ONLY_K_NNF
    
    template < typename Patch, typename DistValue>
    struct NearestNeighborField<Patch, DistValue, 1> : Field2D<true> {

        typedef Patch TargetPatch;
        typedef DistValue DistanceValue;
        typedef typename Patch::point point;
        typedef typename point::vec vec;
        
        // --- provide a common RNG instance -----------------------------------
        RNG rng() const;
        // --- provide a target dimension --------------------------------------
        FrameSize targetSize() const;
        // --- provide patch and distance storage ------------------------------
        TargetPatch &patch(const Point2i &i);
        DistValue &distance(const Point2i &i);
        // --- provide a distance computation ----------------------------------
        DistanceValue dist(const Point2i &pos, const TargetPatch &q);
        // --- provide a filtering for special cases (auto-nnf) ----------------
        bool filter(const Point2i &pos, const TargetPatch &q) const;
    };
    
#endif
    
    template < typename Patch, typename DistValue, int K >
    inline bool isValid(const NearestNeighborField<Patch, DistValue, K> *nnf, const Patch &p) {
        const FrameSize &f = nnf->targetSize();
        Point2i farPixel(Patch::width() - 1, Patch::width() - 1);
        // a patch is valid if the target contains its top-left and bottom-right pixels
        typedef typename Patch::point point;
        return f.contains(point(p)) && f.contains(point(p.transform(farPixel)));
    }
    
    template < typename S, typename DistValue, int K >
    inline bool isValid(const NearestNeighborField<BasicIndexedPatch<S>, DistValue, K> *nnf, const BasicIndexedPatch<S> &p) {
        typedef BasicIndexedPatch<S> TargetPatch;
        const FrameSize &f = nnf->targetSize(p.index);
        Point2i farPixel(TargetPatch::width() - 1, TargetPatch::width() - 1);
        // a patch is valid if the target contains its top-left and bottom-right pixels
        typedef typename TargetPatch::base BasePoint;
        return f.contains(BasePoint(p)) && f.contains(BasePoint(p.transform(farPixel)));
    }
    
}

#endif	/* NNF_H */

