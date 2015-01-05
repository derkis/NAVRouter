//
//  NAVViewControllerFactory.m
//  Navigator
//

#import "NAVViewControllerFactory.h"
#import "NAVViewController.h"

@implementation NAVViewControllerFactory

- (UIViewController *)controllerForRoute:(NAVRoute *)route
{
    // destination should be a controller class in this case
    Class<NAVViewController> klass = route.destination;
     
    NAVViewController *controller;
    
    // attempt to create the view controller; we're only catching exceptions here so that
    // we might throw something more informative
    @try {
        controller = [klass instance];
    }
    // if the storyboard doesn't contain this vc, throw a custom exception
    @catch(NSException *exception) {
        if([exception.name isEqualToString:NSInvalidArgumentException]) {
            exception = [self routingExceptionForRoute:route controllerClass:klass];
        }
        
        [exception raise];
    }
    
    return controller;
}

- (NAVAnimation *)animationForRoute:(NAVRoute *)route
{
    return route.type >= NAVRouteTypeAnimation ? route.destination : nil;
}

//
// Exceptions
//

- (NSException *)routingExceptionForRoute:(NAVRoute *)route controllerClass:(Class<NAVViewController>)klass
{
    NSString *reason = [NSString stringWithFormat:@"%@.storyboard does not contain a view controller with id \"%@\" for route \"%@\"",
        [klass storyboardName], [klass storyboardIdentifier], route.path
    ];
    
    return [NSException exceptionWithName:NAVExceptionViewConfiguration reason:reason userInfo:nil];
}

@end