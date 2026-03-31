/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : snake_core.c
 *    Description : Snake-game core
 *
 *    History :
 *    1. Date        : 28, May 2013
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include "snake_hw.h"
#include "snake_core.h"
#include "snake_display.h"

uint16_t snakeHead;

volatile uint8_t  level;

uint8_t snakeBody[SNAKE_MAX_SEGS];
uint8_t feedPlaces[SNAKE_MAX_FEED_PLACES];
SnakeGameLevel_t snakeLevelData[SNAKE_MAX_LEVELS] = {
  {SNAKE_GO_DELAY_L1},
  {SNAKE_GO_DELAY_L2},
  {SNAKE_GO_DELAY_L3},
  {SNAKE_GO_DELAY_L4},
  {SNAKE_GO_DELAY_L5},
  {SNAKE_GO_DELAY_L6},
  {SNAKE_GO_DELAY_L7},
  {SNAKE_GO_DELAY_L8},
  {SNAKE_GO_DELAY_L9},
};

uint8_t feedCount;
uint8_t exitDoor;

uint16_t score;

uint8_t snakeDirection;

volatile uint8_t  snakeGo;
volatile uint32_t snakeDelayTimer;
volatile uint32_t snakeSeedTimer;
volatile uint32_t snakeGoTimer;
volatile uint32_t snakeReadControlsTimer;
volatile uint8_t  snakeReadControls;

/*************************************************************************
 * Function Name: Snake_Delay100us
 * Parameters:    uint32_t Dly - number of 100us periods to wait
 * Return:        none
 *
 * Description: Delays the program execution Dly*100us.
 *************************************************************************/
static void Snake_Delay100us(uint32_t Dly)
{
  snakeDelayTimer = Dly;
  /* Do controls dummy read */
  while(snakeDelayTimer != 0)
  {
    if(snakeReadControls)
    {
      snakeReadControls = 0;
      Snake_readControls(NULL);
    }
  }
}

/*************************************************************************
 * Function Name: __DelayDecrement
 * Parameters:    none
 * Return:        none
 *
 * Description: Internal function. Decrements the delay counter.
 *************************************************************************/
static void __DelayDecrement(void)
{
  if (snakeDelayTimer != 0x00)
  {
    snakeDelayTimer--;
  }
}

/*************************************************************************
 * Function Name: Snake_Heartbeat
 * Parameters:    none
 * Return:        none
 *
 * Description: Snake's heartbeat. This function should be called every 100us
 *              from an interrupt handler.
 *************************************************************************/
void Snake_Heartbeat(void)
{
  if (++snakeGoTimer >= snakeLevelData[level].snakeGoValue)
  {
    snakeGo = 1;
    snakeGoTimer = 0;
  }
  if (++snakeReadControlsTimer >= SNAKE_CTRL_READ_DELAY)
  {
    snakeReadControls = 1;
    snakeReadControlsTimer = 0;
  }
  snakeSeedTimer+=1000;
  __DelayDecrement();
}

/*************************************************************************
 * Function Name: __Snake_getSeedTimer
 * Parameters:    none
 * Return:        uint32_t - the "seed" timer value
 *
 * Description: Gets value of the seed timer, needed to set the random number
 *              generator seed.
 *************************************************************************/
static uint32_t __Snake_getSeedTimer(void)
{
  return snakeSeedTimer;
}

/*************************************************************************
 * Function Name: __Snake_getGoTimer
 * Parameters:    none
 * Return:        uint32_t - the "snake go" timer value
 *
 * Description: Gets value of the "snake go" timer, needed to set the random
                number generator seed.
 *************************************************************************/
static uint32_t __Snake_getGoTimer(void)
{
  return snakeGoTimer;
}

/*************************************************************************
 * Function Name: Snake_initLevel
 * Parameters:    uint8_t level - level to init
 * Return:        none
 *
 * Description: Initialize the current level.
 *************************************************************************/
static void Snake_initLevel()
{
  snakeBody[0] =((SNAKE_INIT_POS_X-1)<<4) | SNAKE_INIT_POS_Y+2;
  snakeBody[1] = (SNAKE_INIT_POS_X<<4) | SNAKE_INIT_POS_Y+2;
  snakeBody[2] = (SNAKE_INIT_POS_X<<4) | SNAKE_INIT_POS_Y+1;
  snakeBody[3] = (SNAKE_INIT_POS_X<<4) | SNAKE_INIT_POS_Y;
  snakeHead = 3;
  feedCount = 2;
  exitDoor = 0;
  snakeDirection = SNAKE_DIR_UP;
  snakeGoTimer = 0;
  snakeReadControlsTimer = 0;
  snakeReadControls = 0;
  snakeDelayTimer = 0;
  snakeGo = 0;
}

/*************************************************************************
 * Function Name: Snake_isSnake
 * Parameters:    uint8_t position - position at playgound
 *                uint8_t part - part(s) of the snake
 * Return:        uint8_t result 0 - no parts of snake at the position
 *                               1 - snake parts match
 *
 * Description: Checks if there is(are) part(s) from the snake at the given
 *              position.
 *************************************************************************/
static uint8_t Snake_isSnake(uint8_t position, uint8_t part)
{
  if (part & SNAKE_PART_BODY) for(int i=SNAKE_POS_TAIL+1; i<snakeHead; i++) if(snakeBody[i] == position) return 1;
  if (part & SNAKE_PART_HEAD) if(snakeBody[snakeHead] == position) return 1;
  if (part & SNAKE_PART_TAIL) if(snakeBody[SNAKE_POS_TAIL] == position) return 1;
  return 0;
}

/*************************************************************************
 * Function Name: Snake_getSeed
 * Parameters:    none
 * Return:        uint32_t - seed for random number generator
 *
 * Description: Produces pseudo random seed for the random number generator.
 *************************************************************************/
static uint32_t Snake_getSeed(void)
{
  return __Snake_getSeedTimer() + __Snake_getGoTimer();
}

/*************************************************************************
 * Function Name: Snake_isFeed
 * Parameters:    uint8_t position - position at playgound
 * Return:        uint8_t result 0 - no feed at the position
 *                               1 - feed match
 *
 * Description: Checks if there is feed at the given position.
 *************************************************************************/
static uint32_t Snake_isFeed(uint8_t position)
{
  for(int i = 0; i < feedCount; i++) if(feedPlaces[i] == position) return 1;
  return 0;
}

/*************************************************************************
 * Function Name: Snake_generateFeed
 * Parameters:    uint8_t count - number of feed places to generate
 * Return:        none
 *
 * Description: Generates random placed feed.
 *************************************************************************/
static void Snake_generateFeed(uint8_t count)
{
  uint8_t x;
  uint8_t y;
  uint8_t position;
  feedCount = 0;
  while(feedCount < count)
  {
   x = (rand()%(SNAKE_MAX_X - SNAKE_MIN_X + 1) + SNAKE_MIN_X);
   y = (rand()%(SNAKE_MAX_Y - SNAKE_MIN_Y + 1) + SNAKE_MIN_Y);
   position = ((x & 0x0F)<<4) | (y & 0x0F);
   if (!Snake_isSnake(position, SNAKE_PART_ALL) && !Snake_isFeed(position))
   {
     feedPlaces[feedCount++] = position;
   }
  }
}

/*************************************************************************
 * Function Name: Snake_removeFeed
 * Parameters:    uint8_t position - position at playgound
 * Return:        none
 *
 * Description: Remove the feed from the given position.
 *************************************************************************/
static void Snake_removeFeed(uint8_t position)
{
  for(int i = 0; i < feedCount; i++) if(feedPlaces[i] == position) feedPlaces[i] = 0x00;
}

/*************************************************************************
 * Function Name: Snake_haveAnyFeed
 * Parameters:    none
 * Return:        none
 *
 * Description: Checks if there are any feed on playground.
 *************************************************************************/
static uint32_t Snake_haveAnyFeed()
{
  for(int i = 0; i < feedCount; i++) if(feedPlaces[i] != 0x00) return 1;
  return 0;
}

/*************************************************************************
 * Function Name: Snake_generateExit
 * Parameters:    none
 * Return:        none
 *
 * Description: Generates random placed exit door.
 *************************************************************************/
static void Snake_generateExit()
{
  uint8_t x;
  uint8_t y;
  uint8_t door, currentDoor = 0;
  uint8_t doorPlaces = (SNAKE_MAX_X - SNAKE_MIN_X + 1)*2 + (SNAKE_MAX_Y - SNAKE_MIN_Y + 1)*2;
  door = rand()%doorPlaces;
  for(x=SNAKE_MIN_X-1; x<=SNAKE_MAX_X+1; x++)
  for(y=SNAKE_MIN_Y-1; y<=SNAKE_MAX_Y+1; y++)
  {
    if((y == SNAKE_MIN_Y-1) || (y == SNAKE_MAX_Y+1) || (x == SNAKE_MIN_X-1) || (x == SNAKE_MAX_X+1))
    {
      if(!(((y == SNAKE_MIN_Y-1) && (x == SNAKE_MIN_X-1)) ||
           ((y == SNAKE_MIN_Y-1) && (x == SNAKE_MAX_X+1)) ||
           ((y == SNAKE_MAX_Y+1) && (x == SNAKE_MIN_X-1)) ||
           ((y == SNAKE_MAX_Y+1) && (x == SNAKE_MAX_X+1))))
      {
        if (door == currentDoor++) exitDoor = ((x & 0x0F) << 4) | (y & 0x0F);
      }
    }
  }
}

/*************************************************************************
 * Function Name: Snake_getSegmentType
 * Parameters:    uint8_t segPosition - index of snake's segment
 * Return:        uint8_t - type of snake's segment. See header file for info.
 *
 * Description: Returns type of the snake's segment at the given index position.
 *************************************************************************/
static uint8_t Snake_getSegmentType(uint8_t segPosition)
{
uint8_t x,x1,x2;
uint8_t y,y1,y2;

  x = (snakeBody[segPosition] & 0xF0) >> 4;
  y =  snakeBody[segPosition] & 0x0F;

  if(segPosition != snakeHead)
  {
    x1 = (snakeBody[segPosition+1] & 0xF0) >> 4;
    y1 =  snakeBody[segPosition+1] & 0x0F;
  }

  x2 = (snakeBody[segPosition-1] & 0xF0) >> 4;
  y2 =  snakeBody[segPosition-1] & 0x0F;

  if(segPosition == snakeHead)
  {
    if (((x > SNAKE_MAX_X) || (y > SNAKE_MAX_Y) || (x < SNAKE_MIN_X) || (y < SNAKE_MIN_Y)) && !(snakeBody[segPosition] == exitDoor))
    {
      return SNAKE_SEG_HH;
    }
    if (Snake_isSnake(snakeBody[segPosition], SNAKE_PART_TAIL | SNAKE_PART_BODY))
    {
      return SNAKE_SEG_HH;
    }
    if(y > y2)
    {
      if (x == x2) return SNAKE_SEG_HD;
      if (x != x2) return SNAKE_SEG_INVALID;
    }
    if(y < y2)
    {
      if (x == x2) return SNAKE_SEG_HU;
      if (x != x2) return SNAKE_SEG_INVALID;
    }
    if(y == y2)
    {
      if (x < x2) return SNAKE_SEG_HL;
      if (x > x2) return SNAKE_SEG_HR;
      if (x == x2) return SNAKE_SEG_INVALID;
    }
  }

  if(segPosition == SNAKE_POS_TAIL)
  {
    if(snakeBody[segPosition] == exitDoor)
    {
      if (x > x2) return SNAKE_SEG_TR;
      if (x < x2) return SNAKE_SEG_TL;
      if (y > y2) return SNAKE_SEG_TD;
      if (y < y2) return SNAKE_SEG_TU;
    }
    if(y > y1)
    {
      if (x == x1) return SNAKE_SEG_TU;
      if (x != x1) return SNAKE_SEG_INVALID;
    }
    if(y < y1)
    {
      if (x == x1) return SNAKE_SEG_TD;
      if (x != x1) return SNAKE_SEG_INVALID;
    }
    if(y == y1)
    {
      if (x < x1) return SNAKE_SEG_TR;
      if (x > x1) return SNAKE_SEG_TL;
      if (x == x1) return SNAKE_SEG_INVALID;
    }
  }

  if(snakeBody[segPosition] == exitDoor)
  {
    if (x == x2) return SNAKE_SEG_V;
    if (y == y2) return SNAKE_SEG_H;
  }

  if((y != y1) && (y != y2) && (x == x1) && (x == x2)) return SNAKE_SEG_V;
  if((y == y1) && (y == y2) && (x != x1) && (x != x2)) return SNAKE_SEG_H;

  if(y == y1)
  {
    if(y > y2)
    {
      if(x == x2)
      {
        if(x < x1) return  SNAKE_SEG_RU;
        if(x > x1) return  SNAKE_SEG_LU;
        if(x == x1) return  SNAKE_SEG_INVALID;
      }
    }
    if(y < y2)
    {
      if(x == x2)
      {
        if(x < x1) return  SNAKE_SEG_RD;
        if(x > x1) return  SNAKE_SEG_LD;
        if(x == x1) return  SNAKE_SEG_INVALID;
      }
    }
  }

  if(y == y2)
  {
    if(y > y1)
    {
      if(x == x1)
      {
        if(x < x2) return  SNAKE_SEG_RU;
        if(x > x2) return  SNAKE_SEG_LU;
        if(x == x2) return  SNAKE_SEG_INVALID;
      }
    }
    if(y < y1)
    {
      if(x == x1)
      {
        if(x < x2) return  SNAKE_SEG_RD;
        if(x > x2) return  SNAKE_SEG_LD;
        if(x == x2) return  SNAKE_SEG_INVALID;
      }
    }
  }
  return SNAKE_SEG_INVALID;
}

/*************************************************************************
 * Function Name: Snake_update
 * Parameters:    uint8_t part - part(s) of snake to update/display.
 * Return:        none
 *
 * Description: Updates/displays part(s) of the snake on the screen.
 *************************************************************************/
static void Snake_update(uint8_t part)
{
  if(part & SNAKE_PART_TAIL)
  {
    Snake_drawSegment(snakeBody[SNAKE_POS_TAIL],Snake_getSegmentType(SNAKE_POS_TAIL));
    if(snakeBody[SNAKE_POS_TRAIL] != snakeBody[snakeHead]) Snake_drawSegment(snakeBody[SNAKE_POS_TRAIL],0);
  }

  if(part & SNAKE_PART_BODY)
  {
    for(int i=SNAKE_POS_TAIL+1; i<snakeHead; i++)
    {
      Snake_drawSegment(snakeBody[i],Snake_getSegmentType(i));
    }
  }

  if(part & SNAKE_PART_HEAD)
  {
    Snake_drawSegment(snakeBody[snakeHead-1],Snake_getSegmentType(snakeHead-1));
    Snake_drawSegment(snakeBody[snakeHead],Snake_getSegmentType(snakeHead));
  }
}

/*************************************************************************
 * Function Name: Snake_move
 * Parameters:    uint8_t dir - direction.
 * Return:        uint8_t - result of the movement:
 *                SNAKE_MOVE_HIT - when a heads hits in the body or in the wall
 *                SNAKE_MOVE_EXIT - when heads enters the exit door.
 *                SNAKE_MOVE_FEED - when heads eats some feed.
 *                SNAKE_MOVE_OK - normal move, nothing happens.
 *
 * Description: Makes a single move/grow to the given direction. Returns the
 *              result of the movement.
 *************************************************************************/
static uint8_t Snake_move(uint8_t dir, uint8_t grow)
{
uint8_t x = (snakeBody[snakeHead] & 0xF0) >> 4;
uint8_t y =  snakeBody[snakeHead] & 0x0F;
uint8_t result = SNAKE_MOVE_OK;

  if (Snake_isSnake(snakeBody[snakeHead], SNAKE_PART_TAIL | SNAKE_PART_BODY)) return SNAKE_MOVE_INVALID;

  if(!grow) for(int i=SNAKE_POS_TAIL; i<=snakeHead; i++) snakeBody[i-1] = snakeBody[i];

  switch(dir)
  {
    case SNAKE_DIR_UP:
      if(y == SNAKE_MIN_Y) result = SNAKE_MOVE_HIT;
      y-=1;
      break;
    case SNAKE_DIR_DOWN:
      if(y == SNAKE_MAX_Y) result = SNAKE_MOVE_HIT;
      y+=1;
      break;
    case SNAKE_DIR_LEFT:
      if(x == SNAKE_MIN_X) result = SNAKE_MOVE_HIT;
      x-=1;
      break;
    case SNAKE_DIR_RIGHT:
      if(x == SNAKE_MAX_X) result = SNAKE_MOVE_HIT;
      x+=1;
      break;
  }

  if(grow)
  {
    if(snakeHead < SNAKE_MAX_SEGS) ++snakeHead;
    else return SNAKE_MOVE_INVALID;
  }

  snakeBody[snakeHead] = x<<4 | y;

  if(snakeBody[snakeHead] == exitDoor) result = SNAKE_MOVE_EXIT;

  if (Snake_isFeed(snakeBody[snakeHead]))
  {
    Snake_removeFeed(snakeBody[snakeHead]);
    result = SNAKE_MOVE_FEED;
  }

  if (Snake_isSnake(snakeBody[snakeHead], SNAKE_PART_TAIL | SNAKE_PART_BODY)) result = SNAKE_MOVE_HIT;

  return result;
}

/*************************************************************************
 * Function Name: Snake_exit
 * Parameters:    none
 * Return:        none
 *
 * Description: Animates snake's exiting from the door.
 *************************************************************************/
static void Snake_exit(void)
{
uint8_t f=SNAKE_PART_BODY;
  while(snakeHead>1)
  {
    for(int i=SNAKE_POS_TAIL; i<=snakeHead; i++) snakeBody[i-1] = snakeBody[i];
    Snake_update(SNAKE_PART_TAIL | f);
    snakeHead--;
    Snake_Delay100us(1000);
    f=0;
  }
  Snake_drawExit();
}

/*************************************************************************
 * Function Name: Snake_waitForKey
 * Parameters:    none
 * Return:        uint32_t result of control reading function
 *                SNAKE_READCTRL_RES_OK - Ok
 *                SNAKE_READCTRL_RES_ERROR - an error occured
 *
 * Description: Waits for player's keypress from the control(joystick or kbd).
 *************************************************************************/
static uint32_t Snake_waitForKey(void)
{
ControlsData_t controls;

  do
  {
    if(snakeReadControls)
    {
      snakeReadControls = 0;
      if(Snake_readControls(&controls) == SNAKE_READCTRL_RES_ERROR) return SNAKE_READCTRL_RES_ERROR;
    }
  } while (!controls.key0 && !controls.key1);
  return SNAKE_READCTRL_RES_OK;
}

/*************************************************************************
 * Function Name: Snake_game
 * Parameters:    none
 * Return:        uint32_t - type of game exit
 *                SNAKE_EXIT_COMPLETED - all levels completed
 *                SNAKE_EXIT_FAILED - game failed
 *                SNAKE_EXIT_EXCEPTION - unexpected error occured
 *
 * Description: Main game function. Must be called from the user app.
 *************************************************************************/
uint32_t Snake_game(void)
{
ControlsData_t controls;
uint32_t res;
uint8_t snakeGrowCounter;
uint8_t levelCompleted;
uint8_t snakeDirectionTemp;

  /* Show credits */
  Snake_clearScreen();
  Snake_printMessage(3,0,"IAR SYSTEMS AB.");
  Snake_printMessage(3,2,"PRESENTS");
  Snake_Delay100us(20000);
  Snake_clearScreen();
  Snake_printMessage(3,0,"SNAKE'S");
  Snake_printMessage(3,2,"ADVENTURE");
  Snake_Delay100us(20000);
  Snake_clearScreen();

  level = 0;
  score = 0;

  while(level<SNAKE_MAX_LEVELS)
  {
    levelCompleted = 0;
    snakeGrowCounter = 0;

    /* Set initial control direction to UP */
    snakeDirectionTemp = SNAKE_DIR_UP;

    /* Init level and draw playground */
    Snake_initLevel();
    Snake_drawWalls();
    Snake_drawBackground();
    Snake_printLevel(level+1);
    Snake_printScore(score);

    Snake_printMessage(2,0,"Press any key");
    if(level)
      Snake_printMessage(2,1,"to start level");
    else
      Snake_printMessage(2,1,"to start game");

    Snake_waitForKey();

    /* Init random number generator with random seed */
    if (!level) srand(Snake_getSeed());

    Snake_drawBackground();
    Snake_update(SNAKE_PART_ALL);
    Snake_generateFeed(feedCount);
    Snake_drawFeed();

    /* loop until all levels are completed */
    while(!levelCompleted)
    {
      /* read controls (joystick or keyboard) */
      if(snakeReadControls)
      {
        snakeReadControls = 0;
        if(Snake_readControls(&controls) == SNAKE_READCTRL_RES_ERROR) return SNAKE_EXIT_EXCEPTION;
        if(controls.direction != SNAKE_DIR_NONE) snakeDirectionTemp = controls.direction;
      }

      if(controls.direction != SNAKE_DIR_NONE) snakeDirectionTemp = controls.direction;

      if(snakeGo)
      {
        snakeGo = 0;

        /* Check if direction is not backward */
        if(!(((snakeDirectionTemp == SNAKE_DIR_LEFT)  && (snakeDirection == SNAKE_DIR_RIGHT)) ||
             ((snakeDirectionTemp == SNAKE_DIR_RIGHT) && (snakeDirection == SNAKE_DIR_LEFT))  ||
             ((snakeDirectionTemp == SNAKE_DIR_UP)    && (snakeDirection == SNAKE_DIR_DOWN))  ||
             ((snakeDirectionTemp == SNAKE_DIR_DOWN)  && (snakeDirection == SNAKE_DIR_UP))))
        {
          snakeDirection = snakeDirectionTemp;
        }

        res = Snake_move(snakeDirection, snakeGrowCounter);
        Snake_update(SNAKE_PART_HEAD | (snakeGrowCounter?0:SNAKE_PART_TAIL));

        if (snakeGrowCounter) snakeGrowCounter--;

        switch(res)
        {
          /* If snake's hit occured */
          case SNAKE_MOVE_HIT:
            Snake_waitForKey();
            Snake_drawBackground();
            Snake_drawWalls();
            Snake_printMessage(1,0,"GAME OVER");
            Snake_Delay100us(5000);
            Snake_waitForKey();
            return SNAKE_EXIT_FAILED;
            break;

          /* If snake eats some feed */
          case SNAKE_MOVE_FEED:
            snakeGrowCounter += SNAKE_GROW_SIZE;
            Snake_printScore(++score);
            if (score%SNAKE_FEED_TO_EXIT == 0)
            {
              Snake_generateExit();
              Snake_drawExit();
            }
            else if (!Snake_haveAnyFeed())
            {
              Snake_generateFeed(2);
              Snake_drawFeed();
            }
            break;

          /* If move is invalid (this should not happen) */
          case SNAKE_MOVE_INVALID:
            return SNAKE_EXIT_EXCEPTION;
            break;

          /* If snake exits from the door */
          case SNAKE_MOVE_EXIT:
            Snake_exit();
            levelCompleted = 1;
            break;

          /* Move was OK, and nothing happens */
          case SNAKE_MOVE_OK:
            break;
        }
        /* if this flag is set, it should be cleared in order for control reading to be
           synced to interval multiple by 2ms. Requirement of mod_joystick controller. */
        snakeReadControls = 0;
      }
    }
    level++;
  }
  /* If the game is completed successfully */
  Snake_printMessage(1,0,"Press any key");
  Snake_waitForKey();
  Snake_clearScreen();
  Snake_printMessage(2,0,"CONGRATULATIONS!");
  Snake_printMessage(2,1,"GAME COMPLETED");
  Snake_waitForKey();
  return SNAKE_EXIT_COMPLETED;
}
