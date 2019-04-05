//
//  ARPointRecorder.mm
//  ARKitPointCloudRecorder
//
//  Copyright © 2019 CurvSurf. All rights reserved.
//

#import "ARPointRecorder.h"
#include "PointManager.hpp"

using namespace CurvSurf;

@implementation ARPointRecorder
{
    PointManager manager;
}

- (void) reset
{
    manager.clear();
}

- (void) appendPointsFrom: (const ARPointCloud *) arpc
{
    manager.appendARPointCloud( arpc.count, arpc.points, arpc.identifiers );
}

- (BOOL) saveFullPointsTo: (NSString *) filePath
{
    RESULT_VECTOR result;
    manager.getFullList(result);
    
    return PointManager::saveResultVectorXYZ(result, [filePath UTF8String]) ? YES : NO;
}

- (BOOL) saveAveragePointsTo: (NSString *) filePath
{
    RESULT_VECTOR result;
    manager.getAverageList(result);
    
    return PointManager::saveResultVectorXYZ(result, [filePath UTF8String]) ? YES : NO;
}

- (BOOL) saveDistanceFilterAveragePointsTo: (NSString *) filePath
{
    RESULT_VECTOR result;
    manager.getDistanceFilterList(result);
    
    return PointManager::saveResultVectorXYZ(result, [filePath UTF8String]) ? YES : NO;
}

- (BOOL) saveDistanceFilterAveragePointsTo: (NSString *)filePath zscore: (float) value
{
    RESULT_VECTOR result;
    manager.getDistanceFilterList(result, value);
    
    return PointManager::saveResultVectorXYZ(result, [filePath UTF8String]) ? YES : NO;
}

@end
