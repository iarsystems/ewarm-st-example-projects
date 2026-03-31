/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
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

#ifndef __STEP_MOTOR_DRV_H
#define __STEP_MOTOR_DRV_H

#ifdef  STEP_MOTOR_GLOBAL
#define STEP_MOTOR_EXTERN
#else
#define STEP_MOTOR_EXTERN  extern
#endif

#define ENA_A_MASK  GPIO_Pin_6
#define ENA_B_MASK  GPIO_Pin_2
#define ENA_PORT    GPIOE

#define DEF_SPEED   140

#define MOTOR_STEPS 10

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
 * Parameters: u32 Steps
 *
 * Return: none
 *
 * Description: Step motor run
 *
 *************************************************************************/
void StepMotorRun (u32 Steps);

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
 * Function Name: StepMotorSet
 * Parameters: u32  Speed, Boolean Dir, u32 Steps
 *
 * Return: none
 *
 * Description: Step motor operation set
 *
 *************************************************************************/
void StepMotorSet (u32  Speed, Boolean Dir);

/*************************************************************************
 * Function Name: StepMotorSet
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Return motor state (stop/move)
 *
 *************************************************************************/
Boolean StepMotorDone (void);

/*************************************************************************
 * Function Name: StepMotorDeg2Steps
 * Parameters: f32 Deg
 *
 * Return: u32
 *
 * Description: Convert degrees to steps
 *
 *************************************************************************/
u32 StepMotorDeg2Steps (f32 Deg);

/*************************************************************************
 * Function Name: StepMotorSteps2Deg
 * Parameters: u32 Steps
 *
 * Return: f32
 *
 * Description: Convert steps to degrees
 *
 *************************************************************************/
f32 StepMotorSteps2Deg (u32 Steps);

#endif // __STEP_MOTOR_DRV_H
