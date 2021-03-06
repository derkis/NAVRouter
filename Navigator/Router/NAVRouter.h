//
//  NAVRouter.h
//  Navigator
//

@import Foundation;

#import "NAVRouterDelegate.h"
#import "NAVRouterUpdater.h"
#import "NAVRouterFactory.h"
#import "NAVRouteBuilder.h"
#import "NAVTransitionBuilder.h"

@interface NAVRouter : NSObject

/**
 @brief Sends events concerning the router's update lifecycle. Optional
 
 A user may set the delegate of the router to receive such events, and it also provides a 
 customization point to modify router behavior before it executes.
*/

@property (weak, nonatomic) id<NAVRouterDelegate> delegate;

/**
 @brief Provides an interface for the router to run view stack updates. Required.
 
 The router internally provides a default updater implementation for @c UINavigationController, 
 which can be created explicitly by setting the router's @c navigationController.
 
 @note If the user sets a router's @c delegate to an object that is a navigation
 controller or has a method named @c navigationController, as a convenience the router will
 create an updater from that navigation controller as well.
*/

@property (strong, nonatomic) id<NAVRouterUpdater> updater;

/**
 @brief Creates view controller and animators. Required.
 
 Provides an interface for the router to create these components in order to populate and 
 properly execute routing updates without knowledge of their internals.
*/

@property (strong, nonatomic) id<NAVRouterFactory> factory;

/**
 @brief Indiicates whether a transiiton is currently running
 
 If true, attempted transitions will fail by default. This behavior may be overridden by
 passing the set @c shouldQueue to true on the NAVAttributes passed to
 @c -transitionWithAttributes:animated:completion:.
*/

@property (nonatomic, readonly) BOOL isTransitioning;

/**
 @brief The URL representing the router's current state.
 
 This URL should correspond to the view state of the slice of application managed by this router (provided 
 proper API consumption). The subsequent update will be compared against this URL to determine which updates 
 to run.
*/

@property (nonatomic, readonly) NAVURL *currentUrl;

/**
 @brief The URL the router is transitioning to, if any.
 
 This method return @c nil unless called during a transition, such as from within the router's
 delegate calbacks.
*/

@property (nonatomic, readonly) NAVURL *transitioningUrl;

/**
 @brief Per-class shared instance.
*/

+ (instancetype)router;

/**
 @brief Returns a new transition builder that can be used to run a routing transition
 
 The builder contains a variety of chainable methods for customizing the routing desintation,
 transition attributes, and data passed to routed views.
 
 @see @c NAVTransitionBuilder for a complete list of what's available.
 
 @return A new NAVTransitionBuilder to construct the transition
*/

- (NAVTransitionBuilder *)transition;

/**
 @brief Updates the router's internal routes
 
 Routes may be added at runtime using this method, in addition to the default routes 
 subclasses define in @c -routes:. If a new routes matches the subpath of an exisitng 
 route, the existing route will be overwritten.
 
 @param routingBlock A block that executes the route updates
*/

- (void)updateRoutes:(void(^)(NAVRouteBuilder *route))routingBlock;

/**
 @brief Configures the router to use the navigation controller
 
 The router will use this navigation controller as its updater. This must be called (if that
 behavior is desired) before running the first route.
 
 @param navigationController A navigation controller to use for updates
*/

- (void)setNavigationController:(UINavigationController *)navigationController;

/**
 @brief Forces the router to have the @c url
 
 This should really only be used when manually creating running transitions that would cause
 the router's @c currentUrl to be wrong.
*/

- (void)forceUrl:(NAVURL *)url;

/**
 @brief Resets the router to its default state
 
 Routes are preserved, but the router's current URL is cleared. Subsequent transitions are resolved
 as if the router were brand-new.
*/

- (void)reset;

@end
