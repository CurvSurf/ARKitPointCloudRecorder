//
//  PointManager.hpp
//  ARKitPointCloudRecorder
//
//  Copyright © 2019 CurvSurf. All rights reserved.
//

#ifndef _POINT_MANAGER_H_
#define _POINT_MANAGER_H_

// C++ 11 or higher is required
#include <map>
#include <list>
#include <vector>
#include <stdio.h> // File I/O

#if __APPLE__ // iOS
    #include <simd/simd.h>
    #include <Foundation/Foundation.h>

    #define _ABS(v)       abs(v)
    #define _SQRT(v)      simd::sqrt(v)
    #define _DIST(a,b)    simd_distance((a),(b))
    #define _DIST_SQ(a,b) simd_distance_squared((a),(b))
    #define _ZERO_POINT   simd_make_float3(0.f, 0.f, 0.f)

    typedef uint64_t     point_id_t;
    typedef NSUInteger   point_size_t;
    typedef simd_float3  point_type_t;
#else // Android -> glm is required
    #define GLM_FORCE_RADIANS 1
    #include <glm.hpp>

    #define _ABS(v)       glm::abs(v)
    #define _SQRT(v)      glm::sqrt(v)
    #define _DIST(a,b)    glm::distance(glm::vec3(a),glm::vec3(b))
    #define _DIST_SQ(a,b) glm::distance2(glm::vec3(a),glm::vec3(b))
    #define _ZERO_POINT   glm::vec4(0.0f)

    typedef int32_t   point_id_t;
    typedef int32_t   point_size_t;
    typedef glm::vec4 point_type_t;
#endif /* __APPLE__ */

namespace CurvSurf
{
    typedef std::list<point_type_t>             ARPOINT_LIST;
    typedef std::map<point_id_t, ARPOINT_LIST>  ARPOINT_MAP;
    typedef std::pair<point_id_t, ARPOINT_LIST> ARPOINT_PAIR;
    typedef std::vector<point_type_t>           RESULT_VECTOR;
    
    class PointManager
    {
    private:
        ARPOINT_MAP m_container;
        
    public:
        inline bool isEmpty() const { return m_container.empty(); }
        
    private:
        inline bool same_point(const point_type_t& a, const point_type_t& b) {
            return _DIST_SQ(a, b) < (FLT_EPSILON * FLT_EPSILON);
        }
        
    public: // container manipulate functions
        inline void clear() { m_container.clear(); }
        inline void appendARPointCloud(point_size_t num_of_points, const void* points, const point_id_t* ids) {
            const point_type_t *pPointList = reinterpret_cast<const point_type_t *>(points);
            ARPOINT_MAP::iterator itor;
            
            for(point_size_t i = 0; i < num_of_points; ++i) {
                itor = m_container.find( ids[i] );
                if( itor != m_container.end() ) {
                    // Check for preventing insert duplicated data (point)
                    if( !same_point( itor->second.back(), pPointList[i] ) ) {
                        itor->second.push_back( pPointList[i] );
                    }
                }
                else { // New ID -> Insert New
                    m_container.insert( ARPOINT_PAIR( ids[i], ARPOINT_LIST(1, pPointList[i]) ) );
                }
            }
        }
        
    public:
        // Get Full Feature Point List inside Map Container
        void getFullList(RESULT_VECTOR &out);
        
        // Get list of points averaged by each ID
        void getAverageList(RESULT_VECTOR &out);
        
        // Get list of points averaged by each ID with filtering
        void getDistanceFilterList(RESULT_VECTOR &out, float zscore = 1.5f);
        
    public: // Save Result Vector to xyz File
        inline static bool saveResultVectorXYZ(const RESULT_VECTOR &result, const char *fileName) {
            FILE *fp = fopen(fileName, "w");
            if( fp == nullptr ) {
                fprintf(stderr, "Failed to open [%s]\n", fileName);
                return false;
            }
            
            for(size_t i = 0 ; i < result.size() ; ++i) {
                fprintf( fp, "%g %g %g\n", result[i].x, result[i].y, result[i].z );
            }
            
            fclose(fp);
            
            return true;
        }
    };
}

#endif /* _POINT_MANAGER_H_ */
