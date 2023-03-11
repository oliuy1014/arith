/*****************************************************************************
 *
 *                 blockavg_operations.c
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
#include "blockavg_operations.h"
#include "structs_and_constants.h"
#include "arith40.h"
#include "assert.h"
#include <stdio.h>
#include <math.h>

#define BCD_LIM 0.3
#define PB_PR_LIM 0.5

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
int get_pb_q(Vcs_T arr[]) 
{
        float sum = 0;
        for (int i = 0; i < NUM_CELLS; i++) {
                assert(arr[i] != NULL);
                sum += arr[i]->pb;
        }
        sum = clamp((sum / NUM_CELLS), PB_PR_LIM, -PB_PR_LIM);
        int pb_q = Arith40_index_of_chroma(sum);
        return pb_q;
}

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
int get_pr_q(Vcs_T arr[]) 
{
        float sum = 0;
        for (int i = 0; i < NUM_CELLS; i++) {
                assert(arr[i] != NULL);
                sum += arr[i]->pr;
        }
        sum = clamp(sum / NUM_CELLS, PB_PR_LIM, -PB_PR_LIM);
        int pr_q = Arith40_index_of_chroma(sum);
        return pr_q;
}

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
int get_a(Vcs_T arr[])
{
        float sum = 0;
        for (int i = 0; i < NUM_CELLS; i++) {
                assert(arr[i] != NULL);
                sum += arr[i]->y;
        }
        float a = sum / NUM_CELLS;
        a = clamp(a, 1, 0);
        int a_q = (int)roundf(A_SCALE * a);
        return a_q;
}

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
int get_b(Vcs_T arr[])
{
        float sum = -arr[0]->y - arr[1]->y + arr[2]->y + arr[3]->y;
        float b = clamp((sum / NUM_CELLS), 0.3, -0.3);
        int b_q = (int)roundf(BCD_SCALE * b);
        return b_q;
}

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
int get_c(Vcs_T arr[])
{
        float sum = -arr[0]->y + arr[1]->y - arr[2]->y + arr[3]->y;
        float c = clamp((sum / NUM_CELLS), 0.3, -0.3);
        int c_q = (int)roundf(BCD_SCALE * c);
        return c_q;
}

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
int get_d(Vcs_T arr[])
{
        float sum = arr[0]->y - arr[1]->y - arr[2]->y + arr[3]->y;
        float d = clamp((sum / NUM_CELLS), 0.3, -0.3);
        int d_q = (int)roundf(BCD_SCALE * d);
        return d_q;
}


/**********clamp**************************************************
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
float clamp(float input, float max, float min)
{
        input = (input <= max) ? input : max;
        input = (input >= min) ? input : min;
        return input;
}

/************************vcs to block functions above************************/

/************************block_to_vcs functions below************************/

/**********get_y1*************************************************************
 *
 * Purpose:
 *      Calculates y1 from a, b, c and d 
 * Parameters: 
 *      Block_T block data struct that contain a, b, c, d.
 * Return: 
 *      float y1
 * Expects:
 *       block_data is not null
 * Notes:
 *      Will CRE if block_data is NULL.
 *      Clamps y1 within range [0, 1].
 ************************/
float get_y1(Block_T block_data)
{
        assert(block_data != NULL);
        float a = (float) block_data->a;
        float b = (float) block_data->b;
        float c = (float) block_data->c;
        float d = (float) block_data->d;
        float y = (a / A_SCALE) - (b / BCD_SCALE) -
                  (c / BCD_SCALE) + (d / BCD_SCALE);
        return clamp(y, 1.0, 0.0);
}

/**********get_y2*************************************************************
 *
 * Purpose:
 *      Calculates y2 from a, b, c and d 
 * Parameters: 
 *      Block_T block data struct that contain a, b, c, d.
 * Return: 
 *      float y2
 * Expects:
 *       block_data is not null
 * Notes:
 *      Will CRE if block_data is NULL.
 *      Clamps y2 within range [0, 1].
 ****************************************************************************/
float get_y2(Block_T block_data)
{
        float a = (float) block_data->a;
        float b = (float) block_data->b;
        float c = (float) block_data->c;
        float d = (float) block_data->d;
        float y = (a / A_SCALE) - (b / BCD_SCALE) +
                  (c / BCD_SCALE) - (d / BCD_SCALE);
        return clamp(y, 1.0, 0.0);
}

/**********get_y3*************************************************************
 *
 * Purpose:
 *      Calculates y3 from a, b, c and d 
 * Parameters: 
 *      Block_T block data struct that contain a, b, c, d.
 * Return: 
 *      float y3
 * Expects:
 *       block_data is not null
 * Notes:
 *      Will CRE if block_data is NULL.
 *      Clamps y3 within range [0, 1].
 ****************************************************************************/
float get_y3(Block_T block_data)
{
        float a = (float) block_data->a;
        float b = (float) block_data->b;
        float c = (float) block_data->c;
        float d = (float) block_data->d;
        float y = (a / A_SCALE) + (b / BCD_SCALE) -
                  (c / BCD_SCALE) - (d / BCD_SCALE);
        return clamp(y, 1.0, 0.0);
}

/**********get_y4*************************************************************
 *
 * Purpose:
 *      Calculates y4 from a, b, c and d 
 * Parameters: 
 *      Block_T block data struct that contain a, b, c, d.
 * Return: 
 *      float y4
 * Expects:
 *       block_data is not null
 * Notes:
 *      Will CRE if block_data is NULL.
 *      Clamps y4 within range [0, 1].
 ****************************************************************************/
float get_y4(Block_T block_data)
{
        float a = (float) block_data->a;
        float b = (float) block_data->b;
        float c = (float) block_data->c;
        float d = (float) block_data->d;
        float y = (a / A_SCALE) + (b / BCD_SCALE) +
                  (c / BCD_SCALE) + (d / BCD_SCALE);
        return clamp(y, 1.0, 0.0);
}