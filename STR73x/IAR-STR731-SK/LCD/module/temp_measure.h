/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : temp_measure.h
 *    Description : Temperature measure include file
 *
 *    History :
 *    1. Date        : Nov. 2, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include "includes.h"

#ifndef __TEMP_MEAS_H
#define __TEMP_MEAS_H

/*************************************************************************
 * Function Name: InitTermMeasure
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init NTC analog channel
 *
 *************************************************************************/
void InitTermMeasure (void);

/*************************************************************************
 * Function Name: MeasureTerm
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Measure from NTC channel and filtering the result
 *
 *************************************************************************/
Int32U MeasureTerm (void);

#endif    /* __TEMP_MEAS_H */
