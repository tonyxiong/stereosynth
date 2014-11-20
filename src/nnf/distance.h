/* 
 * File:   distance.h
 * Author: akaspar
 *
 * Created on November 19, 2014, 3:41 PM
 */

#ifndef NNF_DISTANCE_H
#define	NNF_DISTANCE_H

#include "../im/patch.h"
#include "../math/mat.h"

#include <iostream>

namespace pm {
    
    template <typename TargetPatch, typename Scalar>
    using Distance = Scalar(*)(const Image &, const Image &, const typename TargetPatch::SourcePatch &, const TargetPatch &);
    
    namespace dist { 
        
        /**
		 * \brief Simple sum of squared differences
		 */
        template <typename TargetPatch, typename Scalar, int numChannels>
		Scalar SumSquaredDiff(const Image &source, const Image &target,
				const typename TargetPatch::SourcePatch &p1, const TargetPatch &p2) {
            typedef typename TargetPatch::SourcePatch SourcePatch;
            typedef Vec<Scalar, numChannels> Pixel;
			const int width = SourcePatch::width();
			const Scalar invArea = 1.0 / (width * width);
			Scalar sum = 0;
			
			for (const auto &i : p1) {
				Pixel diff = source.at<Pixel>(p1.transform(i)) - target.at<Pixel>(p2.transform(i));
				Scalar d = diff.dot(diff);
				sum += d * invArea;
				if (!std::isfinite(sum)) return sum;
			}
			return sum;
		}
        
        /**
         * Distance identifier
         */
        enum DistanceType {
            SSD,
            Unknown
        };
    }
    
    template <typename Patch, typename Scalar, int channels = 1>
    struct DistanceFactory {
        static Distance<Patch, Scalar> get(dist::DistanceType type, int numChannels){
            if(numChannels > channels){
                return DistanceFactory<Patch, Scalar, channels + 1>::get(type, numChannels);
            }
            switch(type){
                default:
                   std::cerr << "Invalid distance type " << type << "\n";
                case dist::SSD:
                    return &dist::SumSquaredDiff<Patch, Scalar, channels>;
            }
        }
    };
    
#ifndef MAX_SUPPORTED_CHANNELS
#define MAX_SUPPORTED_CHANNELS 1
#endif
    
    template <typename Patch, typename Scalar>
    struct DistanceFactory<Patch, Scalar, MAX_SUPPORTED_CHANNELS+1> {
        static Distance<Patch, Scalar> get(dist::DistanceType, int){
            std::cerr << "We do not supported more than " << MAX_SUPPORTED_CHANNELS << "\n";
            return NULL;
        }
    };
    
}

#endif	/* NNF_DISTANCE_H */
