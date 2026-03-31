/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : arm_spi.c
 *    Description : link from efs to the my functions
 *
 *    History :
 *    1. Date        : October 12, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    $Revision: 39 $
 **************************************************************************/
#include "arm_spi.h"
#include "mmc.h"

/*************************************************************************
 * Function Name: if_initInterface
 * Parameters: hwInterface* file,eint8* opts
 *
 * Return: esint8
 *
 * Description: Init SPI and media
 *
 *************************************************************************/
esint8 if_initInterface(hwInterface* file,eint8* opts)
{
pDiskCtrlBlk_t pStatus;
  MmcStatusUpdate();
  pStatus = MmcDiskStatus();
  switch(pStatus->DiskStatus)
  {
  case DiskCommandPass:
    file->sectorCount = pStatus->BlockNumb;
    return(0);
  case DiskNotPresent:
    return(-1);
  case DiskNotReady:
    return(-2);
  }
  // Unknown error
  return(-3);
}

/*************************************************************************
 * Function Name: if_readBuf
 * Parameters: hwInterface* file,euint32 address,euint8* buf
 *
 * Return: esint8
 *
 * Description: Read from media
 *
 *************************************************************************/
esint8 if_readBuf(hwInterface* file,euint32 address,euint8* buf)
{
  if(MmcDiskIO(buf,address,1,DiskRead) == DiskCommandPass)
  {
    return(0);
  }
  return(1);
}

/*************************************************************************
 * Function Name: if_readBuf
 * Parameters: hwInterface* file,euint32 address,euint8* buf
 *
 * Return: esint8
 *
 * Description: Write from media
 *
 *************************************************************************/
esint8 if_writeBuf(hwInterface* file,euint32 address,euint8* buf)
{
  if(MmcDiskIO(buf,address,1,DiskWrite) == DiskCommandPass)
  {
    return(0);
  }
  return(1);
}

/*************************************************************************
 * Function Name: if_readBuf
 * Parameters: hwInterface* file,euint32 address
 *
 * Return: esint8
 *
 * Description:
 *
 *************************************************************************/
esint8 if_setPos(hwInterface* file,euint32 address)
{
  return(0);
}
