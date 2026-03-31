/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : step_motor_drv.h
 *    Description : Step Motor Driver include file
 *
 *    History :
 *    1. Date        : July 19, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#ifndef __STEP_MOTOR_DRV_2_H
#define __STEP_MOTOR_DRV_2_H

#define PWM_FREQ    20000       /*PWM Frequency*/
#define PWM_RES     (1UL<<(11)) /*PWM Resolution*/

#define RPM_MAX 2000                /*Maximum rotation per minute*/

/*************************************************************************
 * Function Name: StepMotorIntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Step motor interrupt handler
 *
 *************************************************************************/
void StepMotorIntrHandler(void);

/*************************************************************************
 * Function Name: StepMotorInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init Step motor
 *
 *************************************************************************/
void StepMotorInit (void);

/*************************************************************************
 * Function Name: StepMotorRun
 * Parameters: none
 *
 * Return: none
 *
 * Description: Step motor run
 *
 *************************************************************************/
void StepMotorRun ();

/*************************************************************************
 * Function Name: StepMotorStop
 * Parameters: none
 *
 * Return: none
 *
 * Description: Step motor stop
 *
 *************************************************************************/
void StepMotorStop (void);

/*************************************************************************
 * Function Name: StepMotorSetRPM
 * Parameters: float  Rpm
 *
 * Return: none
 *
 * Description: Set Stepper motor Rotation per minute (RPM)
 *              The sign means direction
 *
 *************************************************************************/
void StepMotorSetRPM (float Rpm);


#endif // __STEP_MOTOR_DRV_2_H
