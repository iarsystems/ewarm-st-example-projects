/*******************************************************************************
 *      (c) Copyright IAR System 2013
 *
 *      File name   : controller.c
 *      Description : Used to read the controller and handle controller_t
 *                    struct.
 *
 *      History :
 *      1. Date        : 9, July 2013
 *         Author      : Stefan Risberg
 *         Description : Create
 *
 *      $Revision: 39 $
 ******************************************************************************/

#include "controller.h"
#include "modules/mod_joystick.h"

/*******************************************************************************
 ** Function Name : read_controller
 ** Parameters    : joy - Connection handler to the controller to read.
 **                 cont - Struct to fill with the joystick data.
 ** Returns       : true if read was ok, otherwise false.
 **
 ** Description   : Reads a controller and fills a struct with all the data.
 ******************************************************************************/
bool read_controller(ConnectionHandler_t *joy, controller_t *cont)
{
    uint8_t buf[6];
    uint8_t excTime = 200;

    /* If we are unable to read from the controller a couple of times, read has
     * failed and the controller can or have been disconnected from the UEXT
     * port.
     */
    while(MOD_JOYSTICK_readData(joy, 0x00, buf, 6)) {
        if (excTime) {
            excTime--;
        } else {
	        return false;
        }
    }

    /* Get stick location. */
    cont->stick_x = buf[0];
    cont->stick_y = buf[1];
    /* Check if buttons were pressed. */
    cont->c_btn_pressed = !(buf[5] & 0x02);
    cont->z_btn_pressed = !(buf[5] & 0x01);

    return true;
}

/*******************************************************************************
 ** Function Name : reset_controller
 ** Parameters    : cont - Controller to reset.
 ** Returns       : none.
 **
 ** Description   : Sets default values to controller.
 ******************************************************************************/
void reset_controller(controller_t *cont)
{
    cont->stick_x = 128;
    cont->stick_y = 128;
    cont->z_btn_pressed = false;
    cont->c_btn_pressed = false;
}

