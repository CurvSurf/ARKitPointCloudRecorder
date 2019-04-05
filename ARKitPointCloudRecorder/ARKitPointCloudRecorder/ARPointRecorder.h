//
//  ARPointRecorder.h
//  ARKitPointCloudRecorder
//
//  Copyright © 2019 CurvSurf. All rights reserved.
//

#ifndef _AR_POINT_RECORDER_H_
#define _AR_POINT_RECORDER_H_

#import <ARKit/ARKit.h>

@interface ARPointRecorder : NSObject

- (void) reset;
- (void) appendPointsFrom: (const ARPointCloud *) arpc;

- (BOOL) saveFullPointsTo: (NSString *) filePath;
- (BOOL) saveAveragePointsTo: (NSString *) filePath;
- (BOOL) saveDistanceFilterAveragePointsTo: (NSString *) filePath;
- (BOOL) saveDistanceFilterAveragePointsTo: (NSString *) filePath zscore: (float) value;

@end

#endif /* _AR_POINT_RECORDER_H_ */
