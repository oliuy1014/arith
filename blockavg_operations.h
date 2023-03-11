/*****************************************************************************
 *
 *                 blockavg_operations.h
 *
 *      Assignment: Homework 4 -- arith
 *      Authors: Rivan Mehra (rmehra2), Oliver Uy (ouy01)
 *      Date: March 9th, 2023
 *     
 *      summary:
 *      contains helper math functions to convert a 2x2
 *      block of vcs data to an average block data struct
 *      
 *
 ****************************************************************************/
#include "structs_and_constants.h"

#define NUM_CELLS 4
float get_y1(Block_T block_data);
float get_y2(Block_T block_data);
float get_y3(Block_T block_data);
float get_y4(Block_T block_data);

/*********************block to vcs funcs above*****************************/
/*********************vcs to block funcs below*****************************/

/**********get_pb_q***********************************************************
 *
 * Purpose:
 *      Calculates quantized average pb value from NUM_CELLS Vcs_T structs.
 * Parameters: 
 *      Array of Vcs_T structs.
 * Return: 
 *      int quantized pb value.
 * Expects:
 *      All elements in the Vcs_T array to not be null.
 * Notes:
 *      pb is clamped within the range [-0.5, 0.5] before it is quantized.
 ****************************************************************************/
int get_pb_q(Vcs_T arr[]);

/**********get_pr_q***********************************************************
 *
 * Purpose:
 *      Calculates quantized average pr value from NUM_CELLS Vcs_T structs.
 * Parameters: 
 *      Array of Vcs_T structs.
 * Return: 
 *      int quantized pr value.
 * Expects:
 *      All elements in the Vcs_T array to not be null.
 * Notes:
 *      pr is clamped within the range [-0.5. 0.5] before it is quantized.
 ****************************************************************************/
int get_pr_q(Vcs_T arr[]);

/**********get_a**************************************************************
 *
 * Purpose:
 *      Calculates a for a block from y1, y2, y3, y4
 * Parameters: 
 *      Array of Vcs_T structs.
 * Return: 
 *      int quantized a value.
 * Expects:
 *      All elements in the Vcs_T array to not be null.
 * Notes:
 *      a is clamped within the range [0, 1] before it is quantized.
 ****************************************************************************/
int get_a(Vcs_T arr[]);

/**********get_b**************************************************************
 *
 * Purpose:
 *      Calculates b for a block from y1, y2, y3, y4
 * Parameters: 
 *      Array of Vcs_T structs.
 * Return: 
 *      int quantized b value.
 * Expects:
 *      All elements in the Vcs_T array to not be null.
 * Notes:
 *      b is clamped within the range [-0.3, 0.3] before it is quantized.
 ****************************************************************************/
int get_b(Vcs_T arr[]);

/**********get_c**************************************************************
 *
 * Purpose:
 *      Calculates c for a block from y1, y2, y3, y4
 * Parameters: 
 *      Array of Vcs_T structs.
 * Return: 
 *      int quantized c value.
 * Expects:
 *      All elements in the Vcs_T array to not be null.
 * Notes:
 *      c is clamped within the range [-0.3, 0.3] before it is quantized.
 ****************************************************************************/
int get_c(Vcs_T arr[]);

/**********get_d**************************************************************
 *
 * Purpose:
 *      Calculates d for a block from y1, y2, y3, y4
 * Parameters: 
 *      Array of Vcs_T structs.
 * Return: 
 *      int quantized d value.
 * Expects:
 *      All elements in the Vcs_T array to not be null.
 * Notes:
 *      d is clamped within the range [-0.3, 0.3] before it is quantized.
 ****************************************************************************/
int get_d(Vcs_T arr[]);

/**********clamp************************************************************
 *
 * Purpose:
 *      clamps a value within the range [min, max]
 * Parameters: 
 *      - float input   
 *      - float max
 *      - float min
 * Return: 
 *      float clamped value
 * Expects:
 *      None
 * Notes:
 *      None
 ****************************************************************************/
float clamp(float input, float max, float min);