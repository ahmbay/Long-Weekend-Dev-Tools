//
//  LWEViewAnimationUtils.m
//  jFlash
//
//  Created by Mark Makdad on 6/2/10.
//  Copyright 2010 Long Weekend Inc. All rights reserved.
//

#import "LWEViewAnimationUtils.h"
#define kAnimationKey @"transitionViewAnimation"

//! Helper class to take some of the heavy lifting out of animating views
@implementation LWEViewAnimationUtils


//! Translation animate a view from current location to new location by adding the values contained in point
+ (void) translateView:(UIView*)view byPoint:(CGPoint)point withInterval:(float)delay
{
  [UIView beginAnimations:nil context:nil];
  [UIView setAnimationDuration:delay];
  CGAffineTransform transform = CGAffineTransformMakeTranslation(point.x, point.y);
  view.transform = transform;
  [UIView commitAnimations];
}

// Transition between cards after a button has been pressed
+ (void) doViewTransition:(NSString *)transition direction:(NSString *)direction duration:(float)duration objectToTransition:(UIViewController *)controllerToTransition
{
	UIView *theWindow = [controllerToTransition.view superview];
	[UIView beginAnimations:nil context:NULL];
  
	// set up an animation for the transition between the views
	CATransition *animation = [CATransition animation];
	[animation setDelegate:controllerToTransition];
	[animation setDuration:duration];
	[animation setType:transition];
	[animation setSubtype:direction];
  [[theWindow layer] addAnimation:animation forKey:kAnimationKey];
	[UIView commitAnimations];
}

//! Fades in a given view over a given duration.  The view ends up with an alpha of 1 and IS added to the superview
+ (void) fadeInView:(UIView*)theView intoView:(UIView*)superview fadeDuration:(CGFloat)duration
{
  [superview addSubview:theView];
  [theView setAlpha:0];
  [UIView beginAnimations:nil context:NULL];
  [UIView setAnimationDuration:duration];
  [theView setAlpha:1];
  [UIView commitAnimations];
}

//! Fades out a given view over a given duration.  The view ends up with an alpha of 0 but is NOT removed from the superview
+ (void) fadeOutView:(UIView*)theView fadeDuration:(CGFloat)duration
{
  // fade the view out to alpha 0 over a duration
  [UIView beginAnimations:nil context:NULL];
  [UIView setAnimationDuration:duration];
  [theView setAlpha:0];
  [UIView commitAnimations];
}




@end
