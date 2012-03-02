/**
\file
\author Barrett Edwards

FILE:				MatrixLibrary.c
VERSION:			
AUTHOR:				Barrett Edwards
CREATED:			13 July 2007
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	MatrixLibrary.h
DESCRIPTION:		See MatrixLibrary.h

	Related Functions & Groups
	- Allocation
		- merror	mInit(Mat *mat, int rows, int cols, int type );
		- merror	mFree(Mat *mat);
		- void		mPrint(Mat* m, char* msg);
	- Get & Set
		- merror	mSetu(Mat* mat, int row, int col, uint32 value);
		- merror	mSet(Mat* mat, int row, int col, float value);
		- uint32	mGetu(const Mat* mat, int row, int col);
		- float		mGet(const Mat* mat, int row, int col);
	- Basic Opperations
		- merror 	mZero( Mat* mat );
		- merror 	mSetIdentity( Mat* mat);
		- merror 	mSetAll( Mat* mat, float value);
		- merror	mScale(Mat* mat, float scale);
		- merror	mCopy( const Mat* src, Mat* dst );
		- merror	mTranspose( const Mat* src, Mat* dst );
		- merror	mReshape(Mat* mat, int new_rows, int new_cols);
		- merror	mMinMaxLoc( const Mat* mat, int index, int dimension, float* min_val, float* max_val, int* min_loc, int* max_loc);
		- merror	mCountNonZero( const Mat *mat, int index, int dimension, uint32* count);
	- Basic Algebra
		- float		mTrace( const Mat *mat );
		- merror	mSum(const Mat *mat, int index, int dimension, float* sum);
		- merror 	mMean(const Mat *mat, int index, int dimension, float* mean);
		- TODO		mMode();
		- TODO      mMedian(const Mat *mat, int index, int dimension, float* median, float* min_val, float* max_val );
		- merror	mStdev( const Mat *mat, int index, int dimension, float* mean, float* std );
		- merror	mGEMM(Mat* src1, Mat* src2, float alpha, Mat* dst , uint32 flags );
		- merror	mElem( const Mat* src1, const Mat* src2, Mat* dst, const float scaler, const uint32 bit_vector, int op);
	- Linear Algebra
		- merror	mSVD(Mat* A, Mat* UT, Mat* W, Mat* VT);
		- TODO 		mEigenVV();
		- TODO		mSolve();
		- TODO		mInvert();
		- TODO		mDet();
		- TODO		mNormalize();
		- TODO		mDot();
		- TODO		mCross();

CHANGE LOG

*/ 

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "Serial.h"
#include "State.h"
#include "MatrixLibrary.h"




/* Macros -------------------------------------------------------------------*/
/* Enumerations -------------------------------------------------------------*/
/* Structs ------------------------------------------------------------------*/
/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
/* Code ---------------------------------------------------------------------*/




/** Allocate data buffer & Initialize matrix
	This #will not# allocate memory for the matrix header struct. That must be
	done by the caller function. This init function #will# allocate memory for
	the data buffer by checking out a Buffer from the BufferStore.

	STEPS:
	-# Test if input parameters are correct
	-# Determine data width in bytes
	-# Determine required Buffer type
	-# Checkout the Buffer and initialize the Mat struct
*/
merror mInit(Mat *mat, int rows, int cols, int type)
{
	int data_width;
	int required_num_bytes;
	int required_buffer_type;


	// STEP: Test if input parameters are correct
	if(mat == NULL)
		return mError(MAT_ERROR_NULL_PTR);

	if(cols <= 0 || rows <= 0)
		return mError(MAT_ERROR_INVALID_SIZE);


	// STEP: Determine data width in bytes		
	switch(type){
	default:		return mError(MAT_ERROR_INVALID_TYPE);
	case MAT_8U:	data_width = sizeof(uint8);		break;
	case MAT_8S:	data_width = sizeof(int8);		break;
	case MAT_16U:	data_width = sizeof(uint16);	break;
	case MAT_16S:	data_width = sizeof(int16);		break;
	case MAT_32U:	data_width = sizeof(uint32);	break;
	case MAT_32S:	data_width = sizeof(int32);		break;	
	case MAT_32F:	data_width = sizeof(float);		break;
	case MAT_64F:	data_width = sizeof(double);	break;
	}


	// STEP: Determine required Buffer type
	required_num_bytes	 = rows * cols * data_width;
	required_buffer_type = BS_MAT_XSMALL; 
	if(required_num_bytes > (BS_MAT_DATA_SIZE * LIST_SIZE_MAT_XSMALL)) required_buffer_type = BS_MAT_SMALL;
	if(required_num_bytes > (BS_MAT_DATA_SIZE * LIST_SIZE_MAT_SMALL))  required_buffer_type = BS_MAT_MEDIUM;
	if(required_num_bytes > (BS_MAT_DATA_SIZE * LIST_SIZE_MAT_MEDIUM)) required_buffer_type = BS_MAT_LARGE;
	if(required_num_bytes > (BS_MAT_DATA_SIZE * LIST_SIZE_MAT_LARGE))  return mError(MAT_ERROR_OUT_OF_MEMORY);
	

	// STEP: Checkout the Buffer and initialize the Mat struct
	mat->buffer = BSCheckOut(required_buffer_type);
	if(mat->buffer == NULL)
	{
		return mError(MAT_ERROR_OUT_OF_MEMORY);
	}

	mat->capacity = mat->buffer->capacity; 
	if(rows * cols * data_width > mat->capacity)
	{
		mat->capacity = 0;
		BSCheckIn(&mat->buffer);
		return mError(MAT_ERROR_OUT_OF_MEMORY);
	}

	mat->data.vptr	= mat->buffer->data.voidptr;
	mat->type		= type;
	mat->cols		= cols;
	mat->rows		= rows;
	mat->step		= cols * data_width;	

	return MAT_OK;
}




/** De-Allocate data buffer of a matrix
	This function basically checksin the data Buffer back into the BufferStore.
*/
merror mFree(Mat *mat)
{
	if(mat == NULL || mat->buffer == NULL)
		return mError(MAT_ERROR_NULL_PTR);

	BSCheckIn(&mat->buffer);

	mat->data.vptr  = NULL;
	mat->cols		= 0;
	mat->rows		= 0;
	mat->step		= 0;
	mat->capacity	= 0;

	return MAT_OK;
}




/** Handle a matrix error 
	Write error value to state register and increment the number of mat errors
	that we have received
*/
merror mError(merror error)
{	
	if(error != MAT_OK)
	{ 
		wruint(U_MAT_NUM_ERRORS, rduint(U_MAT_NUM_ERRORS)+1);
		wruint(U_MAT_ERROR, error);

#ifdef SIMULATOR
		printf("Mat Error:%d\r\n",error); 
#endif
	}

	return error;
}




/** Print out a matrix to the GUI or command line
*/
void mPrint(Mat* m, char* msg, int inv)
{
#ifndef SIMULATOR
	{
		int i,j;
		
		if(m == NULL || msg == NULL)
			return;// MAT_ERROR_NULL_PTR;

		Println();

		if(msg != NULL)
		{
			Print(msg);
			if(m->cols > 1)
				Println();
		}
		
		
		for(i=0 ; i < m->rows ; i++)
		{
			for(j=0 ; j < m->cols ; j++)
			{
				if(m->type == MAT_32F || m->type == MAT_64F) 
				{
					PrintFloat(mGet(m,i,j));
					Print("\t");
				}
				else
				{
					PrintFloat(mGetu(m,i,j));
					Print("\t");
				}
			}
			if(m->cols > 1)
				Println();
		}
		if(m->cols == 1)
			Println();
	}
#endif
#ifdef SIMULATOR
	{
		int i,j,iend,jend;

		printf("\r\n");
	
		iend = (inv == 0) ? m->rows : m->cols;
		jend = (inv == 0) ? m->cols : m->rows;

		if(msg != NULL)
		{
			printf(msg);
			if(jend > 1)
				printf("\r\n");
		}
		
		for(i = 0 ; i < iend ; i++)
		{
			for(j = 0 ; j < jend ; j++)
			{
				if(inv == 0)
					printf("%16.5f\t",mGet(m,i,j));
				else
					printf("%16.5f\t",mGet(m,j,i));
			}

			if(jend > 1)
				printf("\r\n");
		}

		if(jend == 1)
			printf("\r\n");
	}
#endif
}



/** Assign an individual element of a 2D matrix non-floating point matrix
	Performs error checking on input parameters and returns an error code if 
	there is a problem. 

	This function (along with mGetu()) should be used only with non-floating 
	point matricies. Floating point matricies should use mSet() and mGet(). 
	These functions were included to avoid casting data back and forth from 
	integer values to floating point, which could possibly cause data precision
	loss.  
*/
merror mSetu(Mat* mat, int row, int col, uint32 value)
{
	int index;

	// Error checking
	if(mat == NULL)
		return mError(MAT_ERROR_NULL_PTR);
	if(row >= mat->rows)
		return mError(MAT_ERROR_ROW_OUT_OF_BOUNDS);
	if(col >= mat->cols)
		return mError(MAT_ERROR_ROW_OUT_OF_BOUNDS);


	// Assign value based on matrix type
	index = row * mat->cols + col;
	switch(mat->type)
	{
	case MAT_8U:	mat->data.uc[index] = (uint8)  value;	break;
	case MAT_8S:	mat->data.c[ index] = (int8)   value;	break;
	case MAT_16U:	mat->data.us[index] = (uint16) value;	break;
	case MAT_16S:	mat->data.s[ index] = (int16)  value;	break;
	case MAT_32U:	mat->data.ui[index] = (uint32) value;	break;
	case MAT_32S:	mat->data.i[ index] = (int32)  value;	break;	
	case MAT_32F:	mat->data.fl[index] = (float)  value;	break;
	case MAT_64F:	mat->data.db[index] = (double) value;	break;
	default:		return MAT_ERROR_INVALID_TYPE;
	}

	return MAT_OK;
}	




/** Assign an individual element of a 2D matrix floating point matrix
	Performs error checking on input parameters and returns an error code if 
	there is a problem. 

	This function (along with mGet()) should be used only with floating 
	point matricies. Non-floating point matricies should use mSetu() and mGetu(). 
	These functions were included to avoid casting data back and forth from 
	integer values to floating point, which could possibly cause data precision
	loss.  
*/
merror mSet(Mat* mat, int row, int col, float value)
{	
	int index;
		
	// Error checking
	if(mat == NULL)
		return mError(MAT_ERROR_NULL_PTR);

	if(row >= mat->rows)
		return mError(MAT_ERROR_ROW_OUT_OF_BOUNDS);

	if(col >= mat->cols)
		return mError(MAT_ERROR_ROW_OUT_OF_BOUNDS);

	// Assign value based on matrix type
	index = row * mat->cols + col;
	switch(mat->type){	
	case MAT_8U:	mat->data.uc[index] = (uint8)  value;	break;
	case MAT_8S:	mat->data.c[ index] = (int8)   value;	break;
	case MAT_16U:	mat->data.us[index] = (uint16) value;	break;
	case MAT_16S:	mat->data.s[ index] = (int16)  value;	break;
	case MAT_32U:	mat->data.ui[index] = (uint32) value;	break;
	case MAT_32S:	mat->data.i[ index] = (int32)  value;	break;
	case MAT_32F:	mat->data.fl[index] = (float)  value;	break;
	case MAT_64F:	mat->data.db[index] = (double) value;	break;
	default:		return MAT_ERROR_INVALID_TYPE;
	}
	
	return MAT_OK;
}	




/** Return an individual element of a 2D matrix non-floating point matrix
	Performs error checking on input parameters and returns an error code if 
	there is a problem. 

	This function (along with mSetu()) should be used only with non-floating 
	point matricies. Floating point matricies should use mSet() and mGet(). 
	These functions were included to avoid casting data back and forth from 
	integer values to floating point, which could possibly cause data precision
	loss.  
*/
uint32 mGetu(const Mat* mat, int row, int col)
{
	int index;
	uint32 return_value;

	// Error checking
	if(mat == NULL || row >= mat->rows || col >= mat->cols)
		return 0;

	return_value = 0;
	index = row * mat->cols + col;
	switch(mat->type){	
	case MAT_8U:	
	case MAT_8S:	return_value = mat->data.uc[index];		break;
	case MAT_16U:	
	case MAT_16S:	return_value = mat->data.us[index];		break;
	case MAT_32U:	
	case MAT_32S:	return_value = mat->data.ui[index];		break;
	}

	return return_value;
}	




/** Return an individual element of a 2D matrix floating point matrix
	Performs error checking on input parameters and returns an error code if 
	there is a problem. 

	This function (along with mSet()) should be used only with floating 
	point matricies. Non-floating point matricies should use mSetu() and mGetu(). 
	These functions were included to avoid casting data back and forth from 
	integer values to floating point, which could possibly cause data precision
	loss.  
*/
float mGet(const Mat* mat, int row, int col)
{
	int index;
	float return_value;
	
	// Error checking
	if(mat == NULL || row >= mat->rows || col >= mat->cols)
		return 0;


	return_value = 0;
	index = row * mat->cols + col;		
	switch(mat->type){
	default:
	case MAT_8U:	return_value = mat->data.uc[index];	break;
	case MAT_8S:	return_value = mat->data.c[ index];	break;
	case MAT_16U:	return_value = mat->data.us[index];	break;
	case MAT_16S:	return_value = mat->data.s[ index];	break;
	case MAT_32U:	return_value = mat->data.ui[index];	break;	
	case MAT_32S:	return_value = mat->data.i[ index];	break;	
	case MAT_32F:	return_value = mat->data.fl[index];	break;	
	case MAT_64F:	return_value = mat->data.db[index];	break;	
	}

	return return_value;
}




/** Sets all data in the matrix to zero
	Uses memset() to clear the array.
*/
merror mZero( Mat* mat )
{
	if(mat == NULL)
		return mError(MAT_ERROR_NULL_PTR);

	memset(mat->data.vptr, 0, mat->rows * mat->step);

	return MAT_OK;
}




/** Initializes to the identity matrix
*/
merror mSetIdentity( Mat* mat)
{
	int i;
	
	if(mat == NULL)
		return mError(MAT_ERROR_NULL_PTR);

	mZero(mat);
	for(i = 0 ; i < mat->rows && i < mat->cols ; i++)
		mSet(mat,i,i,1);

	return MAT_OK;
}




/** Sets all values of the matrix to the specified value
*/
merror mSetAll( Mat* mat, float value)
{
	int i,j;
	
	if(mat == NULL)
		return mError(MAT_ERROR_NULL_PTR);
	
	for(i = 0 ; i < mat->rows ; i++)
		for(j = 0 ; j < mat->cols ; j++)
			mSet(mat,i,j, value);	

	return MAT_OK;
}




/** Scales all values of the matrix by the specified value
*/
merror mScale(Mat* mat, float scale)
{
	int i,j;
	
	if(mat == NULL)
		return mError(MAT_ERROR_NULL_PTR);
	
	for(i = 0 ; i < mat->rows ; i++)
		for(j = 0 ; j < mat->cols ; j++)
			mSet(mat,i,j, scale * mGet(mat,i,j));	

	return MAT_OK;
}




/** Copies one matrix to another.
	Matricies must be the same size and type
*/
merror mCopy( const Mat* src, Mat* dst )
{
	const Mat	*A = src;
	Mat			*D = dst;

	if(src == NULL || dst == NULL)
		return mError(MAT_ERROR_NULL_PTR);

	// Are the matrices the same type?
	if(A->type != D->type)
	{
		mZero(D);
		return mError(MAT_ERROR_MISMATCHED_TYPES);
	}

	// Are the matrices the same size?
	if(A->cols != D->cols || A->rows != D->rows)
	{
		mZero(D);
		return mError(MAT_ERROR_MISMATCHED_SIZES);
	}

	memcpy(D->data.vptr, A->data.vptr, A->rows * A->step);

	return MAT_OK;
}




/** The transposes matrix src into dst
	Matricies must be the same type and appropriatly sized.
	dst(i,j)=src(j,i)
*/
merror mTranspose( const Mat* src, Mat* dst )
{
	int i,j;
	const Mat *A = src;
	Mat *D = dst;
	
	// Error checking
	if(src == NULL || dst == NULL)
		return mError(MAT_ERROR_NULL_PTR);


	// Are the matrices the same type?
	if(A->type != D->type)
	{
		mZero(D);
		return mError(MAT_ERROR_MISMATCHED_TYPES);
	}

	// Are the matrices the same size?
	if(A->cols != D->cols || A->rows != D->rows)
	{
		mZero(D);
		return mError(MAT_ERROR_MISMATCHED_SIZES);
	}


	// Transpose the matrix
	for(i = 0 ; i < A->rows ; i++)
		for(j = 0 ; j < A->cols ; j++)
			mSet(D,j,i, mGet(A,i,j));	

	return MAT_OK;
}




/** Changes shape of matrix/image without copying data.
	This function only changes the number of rows / cols in the matrix. No data
	is moved or copied.
*/
merror mReshape(Mat* mat, int new_rows, int new_cols)
{
	int data_size;

	if(mat == NULL)
		return mError(MAT_ERROR_NULL_PTR);

	// Will the reshaped matrix have the same number of elements
	if((mat->rows*mat->cols) != (new_rows*new_cols))
	{
		mZero(mat);
		return mError(MAT_ERROR_MISMATCHED_SIZES);
	}

	data_size = mat->step / mat->cols;
	mat->step = new_cols * data_size;
	mat->rows = new_rows;
	mat->cols = new_cols;

	return MAT_OK;
}




/** Finds global minimum and maximum value and location in a matrix
	This function returns the location and values of the max and min values in
	the matrix along the specified column or row. 
	
	The values and locations are returned using the pointer parameters 
	(min_val, max_val, min_loc, max_loc). All of these parameters need not be 
	included. If a particular metric is not needed, just pass a NULL pointer 
	in as that return paramter:
	\pre
	mMinMaxLoc( mat, 0, MAT_COL,  &min, NULL, NULL, NULL);
	\endpre. 
	The function basically finds the min / max values and locations, and then 
	if the return paramters are not NULL, it assigns the determined values to
	those pointers.
*/
merror mMinMaxLoc(	const Mat	*mat, 
					int			index, 
					int			dimension, 
					float		*min_val, 
					float		*max_val, 
					int			*min_loc, 
					int			*max_loc)
{
	float min,max,val;
	int i, minloc, maxloc;

	// Error checking
	if(mat == NULL)
		return mError(MAT_ERROR_NULL_PTR);
	

	// Initialization
	min		= 0;
	max		= 0;
	val		= 0;
	i		= 0;
	minloc	= 0;
	maxloc	= 0;


	// Search along the specified row or column
	switch(dimension)
	{
	case MAT_COL: 
		if(index >= mat->cols)
			return mError(MAT_ERROR_COL_OUT_OF_BOUNDS);

		minloc = maxloc = 0;
		max = min = mGet(mat,0,index);

		for(i = 0; i < mat->rows ; i++)
		{
			val = mGet(mat,i,index);
			if(val > max)
			{
				max		= val;
				maxloc	= i;
			}
			if(val < min)
			{
				min		= val;
				minloc	= i;
			}
		}
			
		break;

	case MAT_ROW: 
		if(index >= mat->rows)
			return mError(MAT_ERROR_ROW_OUT_OF_BOUNDS);

		minloc = maxloc = 0;
		max = min = mGet(mat,index,0);

		for(i = 0; i < mat->cols ; i++)
		{
			val = mGet(mat,index,i);
			if(val > max)
			{
				max		= val;
				maxloc	= i;
			}
			if(val < min)
			{
				min		= val;
				minloc	= i;
			}
		}

		break;
	}


	// If return parameters are not null, then assign them
	if(min_val)
		*min_val = min;
	if(max_val)
		*max_val = max;
	if(min_loc)
		*min_loc = minloc;
	if(max_loc)
		*max_loc = maxloc;

	return MAT_OK;
}




/** \brief	Counts the number of non-zero elements in a matrix along the 
			specified row or column.
	The resulting value is returned in the return parameter: uint32 *count.
*/
merror mCountNonZero( const Mat *mat, int index, int dimension, uint32* count)
{
	uint32 local_count;
	int i;

	if(mat == NULL || count == NULL)
		return mError(MAT_ERROR_NULL_PTR);

	local_count=0;
	switch(dimension){
	case MAT_COL: 
		if(index >= mat->cols)
			return mError(MAT_ERROR_COL_OUT_OF_BOUNDS);

		for(i = 0, local_count=0; i < mat->rows ; i++)		
			if(mGet(mat,i,index) != 0)
				local_count++;

		break;

	case MAT_ROW: 
		if(index >= mat->rows)
			return mError(MAT_ERROR_ROW_OUT_OF_BOUNDS);

		for(i = 0, local_count=0 ; i < mat->cols ; i++)		
			if( mGet(mat,index,i) != 0 )
				local_count++;
			 		
		break;
	}

	*count = local_count;

	return MAT_OK;
}




/** Returns trace of matrix.
	Summs the diagonal elements of the matrix. 

	trace(mat) = sum over mat(i,i) 
*/
float mTrace( const Mat *mat )
{
	float sum;
	int i;

	// Error Checking
	if(mat == NULL)
		return 0;//MAT_ERROR_NULL_PTR;
	
	
	for(i = 0, sum = 0 ; i < mat->rows && i < mat->cols ; i++)
		sum += mGet(mat,i,i);

	return sum;
}




/** Summs up the elements along a specified row or column	
*/
merror mSum(const Mat *mat, int index, int dimension, float* sum)
{
	int i;
	float local_sum;
	float *row;
	

	// Error checking
	if(mat == NULL || sum == NULL)
		return mError(MAT_ERROR_NULL_PTR);


	local_sum = 0;	
	switch(dimension)
	{
	case MAT_COL: 
		if(index >= mat->cols)
			return mError(MAT_ERROR_COL_OUT_OF_BOUNDS);

		for(i = 0 ; i < mat->rows ; i++)
		{
			//local_sum += mGet(mat,i,index);
			local_sum += mat->data.fl[i*mat->cols + index];
		}

		break;

	case MAT_ROW: 
		if(index >= mat->rows)
			return mError(MAT_ERROR_ROW_OUT_OF_BOUNDS);

		row = &mat->data.fl[index*mat->cols];
		for(i = 0 ; i < mat->cols ; i++)
		{
			//local_sum += mGet(mat,index,i);
			local_sum += row[i];
		}

		break;
	}

	*sum = local_sum;

	return MAT_OK;
}




/** Determine the Mean along the specified row or column.
	This function first performs the sum along the specified row or column, and
	then divides the sum by the number of elements in that row or column.
	\pre
		mean = mSum() / length
	\endpre
*/
merror mMean(const Mat *mat, int index, int dimension, float* mean)
{
	int len, rv;
	float sum;

	if(mat == NULL || mean == NULL)
		return mError(MAT_ERROR_NULL_PTR);

	len	= (dimension == MAT_COL) ? mat->rows : mat->cols;	
	if(len == 0)
		return mError(MAT_ERROR_DIVIDE_BY_0);

	
	rv = mSum(mat, index, dimension, &sum);
	if(rv)
		return rv;

	if(mean == NULL)
		return mError(MAT_ERROR_NULL_PTR);

	*mean = sum / len;

	return MAT_OK;	
}




/** Determine the Standard Deviation along the specified row or column.	
*/
merror	mStdev( const Mat *mat, int index, int dimension, float* sum, float* mean, float* std )
{
	float	local_mean, 
			local_sum,
			difference;

	float  *row;

	int		i, 
			rv, 
			len;


	if(mat == NULL || std == NULL)
		return mError(MAT_ERROR_NULL_PTR);

	len	= (dimension == MAT_COL) ? mat->rows : mat->cols;	
	if(len == 0)
		return mError(MAT_ERROR_DIVIDE_BY_0);

	rv = mMean(mat, index, dimension, &local_mean);
	if(rv)
		return rv;


	local_sum = 0;
	switch(dimension)
	{
	case MAT_COL: 
		if(index >= mat->cols)
			return mError(MAT_ERROR_COL_OUT_OF_BOUNDS);

		for(i = 0 ; i < mat->rows ; i++)
		{
			//difference = mGet(mat,i,index) - local_mean;
			difference = mat->data.fl[i*mat->cols + index] - local_mean;
			local_sum += difference * difference;
		}

		break;

	case MAT_ROW: 
		if(index >= mat->rows)
			return mError(MAT_ERROR_ROW_OUT_OF_BOUNDS);

		row = &mat->data.fl[index*mat->cols];
		for(i = 0 ; i < mat->cols ; i++)
		{
			//difference = mGet(mat,index,i) - local_mean;
			difference = row[i] - local_mean;
			local_sum += difference * difference;
		}

		break;
	}

	
	if(std)
		*std = sqrt(local_sum / (float)len);

	if(mean)
		*mean = local_mean;

	if(sum)
		*sum = local_sum;

	return MAT_OK;
}




/** Multiply two matricies together (according to matrix algebra)
	This is faster than mGEMM().
	
	\pre
	dst = src1 * src2;
	\endpre
*/
merror mMul(Mat* src1, Mat* src2, Mat* dst)
{
	int i, j, k, iend, jend, kend;
	int astep, bstep, dstep;
	float sum;
	float *arow;
	Mat *A = src1;
	Mat *B = src2;
	Mat *D = dst;

	// this currently only works for floats. need to add case statement
	if(src1->type != MAT_32F)
	{
		mZero(D);
		return mError(MAT_ERROR_MISMATCHED_TYPES);
	}



	// Are ALL the matrices the same type?
	if(A->type != B->type || A->type != D->type )
	{
		mZero(D);
		return mError(MAT_ERROR_MISMATCHED_TYPES);
	}


	// Are A and B multiliable?
	if(A->cols != B->rows)
	{
		mZero(D);
		return mError(MAT_MUL_ERROR_MISMATCHED_SIZE_AB);
	}


	//Does the destination have the corerct number of rows?
	if(A->rows != D->rows)
	{
		mZero(D);
		return mError(MAT_MUL_ERROR_MISMATCHED_ROWS_D);
	}


	//Does the destination have the corerct number of cols?
	if(B->cols != D->cols)
	{
		mZero(D);
		return mError(MAT_MUL_ERROR_MISMATCHED_COLS_D);
	}


	iend = B->cols;
	jend = A->rows;
	kend = A->cols;

	astep = A->cols;
	bstep = B->cols;
	dstep = D->cols;

	/* No transposes. Straight multiply 
	 * indexes:
	 * i = bcol, dcol
	 * j = arow, drow
	 * k = acol, brow
	 */			
	for(i = 0 ; i < iend ; i++)
	{
		for(j = 0 ; j < jend ; j++)
		{		
			arow = &A->data.fl[j*astep]; 
			for(k = 0, sum = 0 ; k < kend ; k++)
			{
				sum += arow[k] * B->data.fl[k*bstep + i];
				//sum += mGet(A,j,k) * mGet(B,k,i);
			}
			D->data.fl[j*dstep + i] = sum;
			//mSet(D,j,i,sum);
		}
	}

	return MAT_OK;
}




/** Multiply two matricies together (according to matrix algebra) and subtract a 3rd matrix
	This is faster than mGEMM().
	
	\pre
	dst = src1 * src2 - src3;
	\endpre
*/
merror mMulSub(Mat* src1, Mat* src2, Mat* src3, Mat* dst)
{
	int i, j, k, iend, jend, kend;
	int astep, bstep, cstep, dstep;
	float sum;
	float *arow;
	Mat *A = src1;
	Mat *B = src2;
	Mat *C = src3;
	Mat *D = dst;

	// this currently only works for floats. need to add case statement
	if(src1->type != MAT_32F)
	{
		mZero(D);
		return mError(MAT_ERROR_MISMATCHED_TYPES);
	}



	// Are ALL the matrices the same type?
	if(A->type != B->type || A->type != C->type || A->type != D->type)
	{
		mZero(D);
		return mError(MAT_ERROR_MISMATCHED_TYPES);
	}


	// Are A and B multiliable?
	if(A->cols != B->rows)
	{
		mZero(D);
		return mError(MAT_MUL_ERROR_MISMATCHED_SIZE_AB);
	}


	//Does the destination have the corerct number of rows?
	if(A->rows != D->rows)
	{
		mZero(D);
		return mError(MAT_MUL_ERROR_MISMATCHED_ROWS_D);
	}


	//Does the destination have the corerct number of cols?
	if(B->cols != D->cols)
	{
		mZero(D);
		return mError(MAT_MUL_ERROR_MISMATCHED_COLS_D);
	}


	iend = B->cols;
	jend = A->rows;
	kend = A->cols;

	astep = A->cols;
	bstep = B->cols;
	cstep = C->cols;
	dstep = D->cols;

	/* No transposes. Straight multiply 
	 * indexes:
	 * i = bcol, dcol
	 * j = arow, drow
	 * k = acol, brow
	 */			
	for(i = 0 ; i < iend ; i++)
	{
		for(j = 0 ; j < jend ; j++)
		{			
			arow = &A->data.fl[j*astep];
			for(k = 0, sum = 0 ; k < kend ; k++)
			{
				sum += arow[k] * B->data.fl[k*bstep + i];
				//sum += mGet(A,j,k) * mGet(B,k,i);
			}
			D->data.fl[j*dstep + i] = sum - C->data.fl[j*cstep + i];
			//mSet(D,j,i,sum - mGet(C,j,i));
		}
	}

	return MAT_OK;
}




/** Performs generalized matrix multiplication
	The flags indicate if any of the three matricies should be transposed.

	- Flags:
		- MUL_A_T = Transpose source matrix A
		- MUL_B_T = Transpose source matrix B
		- MUL_D_T = Transpose destination matrix D

	\pre
	dst = alpha*src1T*src2 
	\endpre
*/
merror mGEMM(Mat* src1, Mat* src2, float alpha, Mat* dst , uint32 flags )
{
	int i, j, k, iend, jend, kend;
	float sum;
	Mat *A = src1;
	Mat *B = src2;
	Mat *D = dst;

	// Are ALL the matrices the same type?
	if(A->type != B->type || A->type != D->type )
	{
		mZero(D);
		return mError(MAT_ERROR_MISMATCHED_TYPES);
	}
	
	// Are A and B multiliable?
	{
		int A_dimension = (flags & MUL_A_T) ? A->rows : A->cols;
		int B_dimension = (flags & MUL_B_T) ? B->cols : B->rows;

		if(A_dimension != B_dimension)
		{
			mZero(D);
			return mError(MAT_MUL_ERROR_MISMATCHED_SIZE_AB);
		}
	}

	//Does the destination have the corerct number of rows?
	{
		int A_dimension = (flags & MUL_A_T) ? A->cols : A->rows;
		int D_dimension = (flags & MUL_D_T) ? D->cols : D->rows;

		if(A_dimension != D_dimension)
		{
			mZero(D);
			return mError(MAT_MUL_ERROR_MISMATCHED_ROWS_D);
		}
	}

	//Does the destination have the corerct number of cols?
	{
		int B_dimension = (flags & MUL_B_T) ? B->rows : B->cols;
		int D_dimension = (flags & MUL_D_T) ? D->rows : D->cols;

		if(B_dimension != D_dimension)
		{
			mZero(D);
			return mError(MAT_MUL_ERROR_MISMATCHED_COLS_D);
		}
	}

	
	iend = (flags & MUL_B_T) ? B->rows : B->cols;
	jend = (flags & MUL_A_T) ? A->cols : A->rows;
	kend = (flags & MUL_A_T) ? A->rows : A->cols;

	switch(flags){
	case 0:	
		/* No transposes. Straight multiply 
		 * indexes:
		 * i = bcol, dcol
		 * j = arow, drow
		 * k = acol, brow
		 */			
		for(i = 0 ; i < iend ; i++)
		{
			for(j = 0 ; j < jend ; j++)
			{				
				for(k = 0, sum = 0 ; k < kend ; k++)
					sum += mGet(A,j,k) * mGet(B,k,i);
				mSet(D,j,i,alpha*sum);
			}
		}	
		break;		

	case MUL_A_T:
		/* Transpose: A
		 * indexes:
		 * i = bcol, dcol
		 * j = acol, drow
		 * k = arow, brow
		 */				
		for(i = 0 ; i < iend ; i++)
		{
			for(j = 0 ; j < jend ; j++)
			{
				for(k = 0, sum = 0 ; k < kend ; k++)
					sum += mGet(A,k,j) * mGet(B,k,i);
				mSet(D,j,i,alpha*sum);
			}
		}	
		break;		

	case MUL_B_T:
		/* Transpose: B
		 * indexes:
		 * i = brow, dcol
		 * j = arow, drow
		 * k = acol, bcol
		 */		
		for(i = 0 ; i < iend ; i++)
		{
			for(j = 0 ; j < jend ; j++)
			{
				for(k = 0, sum = 0 ; k < kend ; k++)				
					sum += mGet(A,j,k) * mGet(B,i,k);				
				mSet(D,j,i,alpha*sum);
			}
		}	
		break;

	case MUL_B_T | MUL_A_T:	
		/* Transpose: A,B
		 * indexes:
		 * i = brow, dcol
		 * j = acol, drow
		 * k = arow, bcol
		 */		
		for(i = 0 ; i < iend ; i++)
		{
			for(j = 0 ; j < jend ; j++)
			{
				for(k = 0, sum = 0 ; k < kend ; k++)
					sum += mGet(A,k,j) * mGet(B,i,k);
				mSet(D,j,i,alpha*sum);
			}
		}
		break;

	case MUL_D_T:	
		/* Transpose: D
		 * indexes:
		 * i = bcol, drow
		 * j = arow, dcol
		 * k = acol, brow
		 */	
		for(i = 0 ; i < iend ; i++)
		{
			for(j = 0 ; j < jend ; j++)
			{
				for(k = 0, sum = 0 ; k < kend ; k++)
					sum += mGet(A,j,k) * mGet(B,k,i);
				mSet(D,i,j,alpha*sum);
			}
		}	
		break;

	case MUL_D_T | MUL_A_T:	
		/* Transpose: D,A
		 * indexes:
		 * i = bcol, drow
		 * j = acol, dcol
		 * k = arow, brow
		 */	
		for(i = 0 ; i < iend ; i++)
		{
			for(j = 0 ; j < jend ; j++)
			{
				for(k = 0, sum = 0 ; k < kend ; k++)
					sum += mGet(A,k,j) * mGet(B,k,i);
				mSet(D,i,j,alpha*sum);
			}
		}	
		break;

	case MUL_D_T | MUL_B_T:	
		/* Transpose: D,B
		 * indexes:
		 * i = brow, drow
		 * j = arow, dcol
		 * k = acol, bcol
		 */	
		for(i = 0 ; i < iend ; i++)
		{
			for(j = 0 ; j < jend ; j++)
			{
				for(k = 0, sum = 0 ; k < kend ; k++)
					sum += mGet(A,j,k) * mGet(B,i,k);
				mSet(D,i,j,alpha*sum);
			}
		}	
		break;

	case MUL_D_T | MUL_B_T | MUL_A_T:
		/* Transpose: D,B,A
		 * indexes:
		 * i = brow, drow
		 * j = acol, dcol
		 * k = arow, bcol
		 */	
		for(i = 0 ; i < iend ; i++)
		{
			for(j = 0 ; j < jend ; j++)
			{
				for(k = 0, sum = 0 ; k < kend ; k++)
					sum += mGet(A,k,j) * mGet(B,i,k);
				mSet(D,i,j,alpha*sum);
			}
		}	
		break;
	}

	return MAT_OK;
}




/** Performs specified opperation on matrix data (element by element)
	This function implements several of the various element opperations that 
	can be performed on two or just one matrix. For example, if you wanted to 
	add two matricies together:
	\pre
	mElem(A, B, D, 0, 0, ELEM_ADD);
	\endpre

	All possible operations are listed in the mElementOpperation enumeration.
*/ 
merror mElem( const Mat* src1, const Mat* src2, Mat* dst, const float scaler, const uint32 bit_vector, int op)
{
	int i,j;
	const Mat *A = src1;
	const Mat *B = src2;
	Mat *D = dst;

	if(B != NULL) /*B will be NULL if we are just doing a scaler opperation*/
	{
		// Are ALL the matrices the same type?
		if(A->type != B->type || A->type != D->type )
		{
			mZero(D);
			return mError(MAT_ERROR_MISMATCHED_TYPES);
		}

		// Are the matrices the same size?
		if(    A->cols != B->cols || A->cols != D->cols 
			|| A->rows != B->rows || A->rows != D->rows )
		{
			mZero(D);
			return mError(MAT_ERROR_MISMATCHED_SIZES);
		}
	}
	else
	{
		// Are ALL the matrices the same type?
		if(A->type != D->type )
		{
			mZero(D);
			return mError(MAT_ERROR_MISMATCHED_TYPES);
		}

		// Are the matrices the same size?
		if(    A->cols != D->cols 
			|| A->rows != D->rows )
		{
			mZero(D);
			return mError(MAT_ERROR_MISMATCHED_SIZES);
		}
	}

	
	

	switch(op){
	case ELEM_ADD: 
		for(i = 0 ; i < A->rows ; i++)
			for(j = 0 ; j < A->cols ; j++)
				mSet(D,i,j, mGet(A,i,j) + mGet(B,i,j));	
		break;
	

	case ELEM_ADDS: 
		for(i = 0 ; i < A->rows ; i++)
			for(j = 0 ; j < A->cols ; j++)
				mSet(D,i,j, mGet(A,i,j) + scaler);	
		break;
	

	case ELEM_SUB: 
		for(i = 0 ; i < A->rows ; i++)
			for(j = 0 ; j < A->cols ; j++)
				mSet(D,i,j, mGet(A,i,j) - mGet(B,i,j));	
		break;

	case ELEM_SUBS: 
		for(i = 0 ; i < A->rows ; i++)
			for(j = 0 ; j < A->cols ; j++)
				mSet(D,i,j, mGet(A,i,j) - scaler);	
		break;

	case ELEM_MUL: 
		for(i = 0 ; i < A->rows ; i++)
			for(j = 0 ; j < A->cols ; j++)
				mSet(D,i,j, mGet(A,i,j) * mGet(B,i,j));	
		break;
	
	case ELEM_MULS: 
		for(i = 0 ; i < A->rows ; i++)
			for(j = 0 ; j < A->cols ; j++)
				mSet(D,i,j, mGet(A,i,j) * scaler);	
		break;
	
	case ELEM_DIV: 
		for(i = 0 ; i < A->rows ; i++)
			for(j = 0 ; j < A->cols ; j++)
				mSet(D,i,j, mGet(A,i,j) / mGet(B,i,j));	
		break;

	case ELEM_DIVS: 
		if(scaler == 0)
		{
			mZero(D);
			return mError(MAT_ERROR_DIVIDE_BY_0);
		}
		for(i = 0 ; i < A->rows ; i++)
			for(j = 0 ; j < A->cols ; j++)
				mSet(D,i,j, mGet(A,i,j) / scaler);	
		break;
	

	}
		
	return MAT_OK;
}
