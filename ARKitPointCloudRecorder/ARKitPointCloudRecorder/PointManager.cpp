//
//  PointManager.cpp
//  ARKitPointCloudRecorder
//
//  Copyright © 2019 CurvSurf. All rights reserved.
//

#include "PointManager.hpp"

#define FORCE_INLINE __attribute__((always_inline))

using namespace CurvSurf;

static FORCE_INLINE point_type_t _normalAverage( const ARPOINT_LIST &list ) {
    point_type_t total = _ZERO_POINT;
    for( ARPOINT_LIST::const_iterator itor = list.begin() ; itor != list.end() ; ++itor ) {
        total += *itor;
    }
    return total / static_cast<float>(list.size());
}

static FORCE_INLINE point_type_t _distanceFilterAverage( const ARPOINT_LIST &list, float zscore ) {
    point_type_t mean_point = _normalAverage(list);
    const size_t len = list.size();
    if( len < 5 ) return mean_point; // at least 5 points are required
    
    ARPOINT_LIST::const_iterator itor;
    float distAvg = 0.0f; // distance average
    float distVar = 0.0f; // distance variance
    
    // Get Distance Average & Variance from Mean Point
    for( itor = list.cbegin(); itor != list.cend(); ++itor ) {
        float distSq = _DIST_SQ( *itor, mean_point );
        distVar += distSq;
        distAvg += _SQRT(distSq);
    }
    distAvg /= static_cast<float>(len);
    distVar = (distVar / static_cast<float>(len)) - (distAvg * distAvg);
    
    float cutSigma = zscore * _SQRT(distVar); // _SQRT(distVar) : standrad deviation
    
    size_t cnt = 0;
    point_type_t filtered = _ZERO_POINT;
    for( itor = list.cbegin(); itor != list.cend(); ++itor ) {
        // filter out points too far from mean point by standard deviation
        if( _ABS( _DIST( *itor, mean_point ) - distAvg ) > cutSigma ) { continue; }
        filtered += *itor;
        ++cnt;
    }
    
    return cnt < 3 ? mean_point : (filtered / static_cast<float>(cnt));
}

void PointManager::getFullList(RESULT_VECTOR &out)
{
    RESULT_VECTOR full;
    
    if( !isEmpty() ) {
        for( ARPOINT_MAP::const_iterator itor = m_container.cbegin() ; itor != m_container.cend() ; ++itor ) {
            full.insert( full.end(), itor->second.begin(), itor->second.end() );
        }
    }
    
    out.swap(full);
}

void PointManager::getAverageList(RESULT_VECTOR &out)
{
    RESULT_VECTOR avg( m_container.size(), _ZERO_POINT );
    
    size_t index = 0;
    for( ARPOINT_MAP::const_iterator itor = m_container.cbegin() ; itor != m_container.cend() ; ++itor ) {
        avg[index++] = _normalAverage( itor->second ); // get mean point
    }
    
    out.swap(avg);
}

void PointManager::getDistanceFilterList(RESULT_VECTOR &out, float zscore)
{
    RESULT_VECTOR avg( m_container.size(), _ZERO_POINT );
    
    size_t index = 0;
    for( ARPOINT_MAP::const_iterator itor = m_container.cbegin() ; itor != m_container.cend() ; ++itor ) {
        avg[index++] = _distanceFilterAverage( itor->second, zscore );
    }
    
    out.swap(avg);
}
