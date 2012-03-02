/**
\unit
\author Barrett Edwards

FILE:				MatrixLibrary.h
VERSION:			
AUTHOR:				Barrett Edwards
CREATED:			13 July 2007
PLATFORM:			Helios Computing Platform
FRAMEWORK:			Fundamental Software Framework
COMPANION FILES:	MatrixLibrary.c
DESCRIPTION:		

	The Matrix Library is meant to serve as an OpenCV 'like' matrix 
	implementation. Most things are pretty much the same as OpenCV. 
	
	
	This library has been specifically crafted to run as a part of the Helios 
	Fundamental software Framework. There are three main dependencies of this
	library that are specific to the Helios frameworks. The first is the memory
	allocation & de-allocation in mInit() & mFree(). And the second is in error
	reporting. The third is in mPrint(), which will just print the matrix to 
	the command line if we are in simulaton.

	Examples:
	\pre
	Mat A;
	Mat B;
	Mat C;
	mInit(&A, 3, 3, MAT_32F);
	mInit(&B, 3, 3, MAT_32F);
	mInit(&C, 3, 3, MAT_32F);
	...
	mIdentity(&A);
	   [1 0 0]
	A =[0 1 0]
	   [0 0 1]
	...
	mZero(&B)
	   [0 0 0]
	B =[0 0 0]
	   [0 0 0]
	...
	mSet(&B, 0, 0, 1.0);
	mSet(&B, 1, 1, 2.0); 
	mSet(&B, 2, 2, 3.0);
	   [1 0 0]
	B =[0 2 0]
	   [0 0 3]
	...	
	mGEMM(&A, &B, 2.0, &C, 0);
	   [2 0 0]
	C =[0 4 0]
	   [0 0 6]
	...
	mFree(&A);
	mFree(&B);
	mFree(&C);
	\endpre


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

/* Conditional Inclusion ----------------------------------------------------*/
#ifndef MATRIXLIBRARY_H
#define MATRIXLIBRARY_H




/* Includes -----------------------------------------------------------------*/
#include <stdio.h>

#include "Header.h"
#include "BufferStore.h"




/* Macros -------------------------------------------------------------------*/

/// Most Matrix functions return this merror as an error code
typedef int merror;




/* Enumerations -------------------------------------------------------------*/

/** Indicates the data type of the matrix.
	The most common type used is MAT_32F for a 32-bit float
*/
enum MatType 
{
	MAT_8U,		///< 8-bit unsigned matrix data
	MAT_8S,		///< 8-bit signed matrix data
	MAT_16U,	///< 16-bit unsigned matrix data
	MAT_16S,	///< 16-bit signed matrix data
	MAT_32U,	///< 32-bit unsigned matrix data
	MAT_32S,	///< 32-bit signed matrix data
	MAT_32F,	///< 32-bit single precision floating point data (Helios FPU only does single precision)
	MAT_64F		///< 64-bit double precision floating point data (Helios FPU only does single precision)
};




/** All possbile element opperations used in the mElem() function
	The mElem() function performs one of the following opperations element by
	element on two matricies of the same size. 
	
	This is equivalent to the following in matlab: C = A.*B or C = A + B
	where A,B, and C are all the same size.

	Some of the following opperations are meant for a matrix and a scaler, such
	as C = A * scaler. All of these opperations have the enumeration name end 
	with an 'S'.

	Some of the following opperations are bitwise opps. These should really 
	only be perfomed on integer MatType matricies (i.e. MAT_32U).
*/
enum mElementOpperation 
{
	ELEM_ADD,	///< Per-element addition of two matricies: C= A + B
	ELEM_ADDS,	///< Add a scaler value to each element in a matrix: C= A + scaler
	ELEM_SUB,	///< Per-element subtraction of two matricies: C= A - B
	ELEM_SUBS,  ///< Subract a scaler value from each element in a matrix: C= A - scaler
	ELEM_MUL,	///< Per-element multiplication of two matricies: C = A.*B
	ELEM_MULS,  ///< Multiply each element in a matrix by a scaler value: C= A.* scaler
	ELEM_DIV,	///< Per-element dividion of two matricies: C= A./B
	ELEM_DIVS,  ///< Divide each element in a matrix by a scaler value: C= A./scaler

	ELEM_AND,	///< Per-element bitwise AND of two matricies: C = A & B
	ELEM_ANDS,	///< Bitwise AND each element in a matrix with a scaler: C = A & scaler
	ELEM_OR,	///< Per-element bitwise OR of two matricies: C = A | B
	ELEM_ORS,	///< Bitwise OR each element in a matrix with a scaler: C = A | scaler
	ELEM_XOR,	///< Per-element bitwise XOR of two matricies: C = A ^ B
	ELEM_XORS,	///< Bitwise XOR each element in a matrix with a scaler: C = A ^ scaler
	ELEM_NOT	///< Bitwise NOT each element in a matrix C = ~A
};




/** Matrix transpose flags for mGEMM()
	Each one of these flags will cause mGEMM() to transpose the source or 
	destination maticies in the function.
*/
enum mGEMMFlag 
{
	MUL_A_T = 0x01,	///< Transpose source matrix A
	MUL_B_T	= 0x02, ///< Transpose source matrix B
	MUL_D_T = 0x04  ///< Transpose destination matrix D
};




/**	Matrix error codes returned by matrix functions
	These error codes are returned by a matrix function to indicate that a 
	certain type of error occured durring the matrix opperation 
	(i.e. divide by 0)
*/
enum mMatError 
{
	MAT_OK,								///< No error occured
	MAT_ERROR_NULL_PTR,					///< One of the input parameters was NULL
	MAT_ERROR_OUT_OF_MEMORY,			///< Could not allocate enough data (or buffers) to perform function
	MAT_ERROR_DIVIDE_BY_0,				///< Attempted to divide by zero
	MAT_ERROR_INVALID_TYPE,				///< Attempted to mInit() with invalid mat type (i.e. not in MatType)
	MAT_ERROR_INVALID_SIZE,				///< Attempted to mInit() with invalid row or column size
	MAT_ERROR_MISMATCHED_SIZES,			///< Input parameter matrices are of the wrong size for opperation 
	MAT_ERROR_MISMATCHED_TYPES,			///< Input parameter matrices are of different data types
	MAT_ERROR_COL_OUT_OF_BOUNDS,		///< Attempted to access a column that doesn't exist
	MAT_ERROR_ROW_OUT_OF_BOUNDS,		///< Attempted to access a row that doesn't exist
	MAT_MUL_ERROR_MISMATCHED_SIZE_AB,	///< Input parameter matrices are of the wrong size for opperation 
	MAT_MUL_ERROR_MISMATCHED_ROWS_D,	///< Input parameter matrices are of the wrong size for opperation 
	MAT_MUL_ERROR_MISMATCHED_COLS_D,	///< Input parameter matrices are of the wrong size for opperation 
	MAT_ERROR_SVD,						///< General SVD Error
	MAT_ERROR_SVD_UNDERDETERMINED_SYSTEM///< SVD cannot be run on a underdetermined system (i.e. rows < cols)
};




/** Used to indicate which matrix dimension an opperation should be perfomed on.
	For example, if we wanted to sum up all the elements in column zero we
	would use:
	\pre
	Mat mat
	float sum;
	mInit(&mat, 3, 3, MAT_32F);	
	...
	put some data in the matrix
	...
	merror rv = mSum(&mat, 0, MAT_COL, &sum);	
	\endpre
*/
enum mMatDimension 
{
	MAT_COL = 0,	///< Perform matrix opperation along a column
	MAT_ROW = 1		///< Perform matrix opperation along a row
};




/** Flags to configure mSVD() function
	NOTE: That the mSVD() function by default will return transposed versions
	of U and V.
*/
enum mSVDFlags 
{
    MAT_SVD_MODIFY_A	= 0x01, ///< Input matrix A will be modified (it will not be the same, not even close, when mSVD() is finished)
    MAT_SVD_U_T			= 0x02,	///< Transpose result matrix U 
    MAT_SVD_V_T			= 0x04  ///< Transpose result matrix V
};




/* Structs ------------------------------------------------------------------*/

/** Matrix Header Struct
	This is the struct that defines a matrix. 
*/
struct Mat
{
	int type; ///< Mat element type (char, int, float...)
    int step; ///< Full row length in bytes

	/** Union of pointers to all possible data types
		This is included so that you can access the data in the data array 
		without having to cast a void pointer to say a float pointer.
	*/
    union mat_data_union
    {
		void	*vptr;
		uint8	*ptr;
        int8	*c;
		uint8	*uc;
		int16	*s;
        uint16	*us;
        int32	*i;
		int32	*ui;
        float	*fl;
        double	*db;
    } data; 

	int capacity;	///< Size of data pointer in number of bytes
    int rows;		///< Matrix(MxN): m number of rows 
    int cols;		///< Matrix(MxN): n number of columns

	Buffer	*buffer;///< Pointer the checked out data Buffer 
};
typedef struct Mat Mat; ///< typedef struct Mat Mat;




/* Global Memory  -----------------------------------------------------------*/
/* External Memory-----------------------------------------------------------*/
/* Function Prototypes ------------------------------------------------------*/
// FG: Allocation
merror	mInit(Mat *mat, int rows, int cols, int type );
merror	mFree(Mat *mat);
merror	mError(merror error);
void	mPrint(Mat* m, char* msg, int inv);

// FG: Get & Set
merror	mSetu(Mat* mat, int row, int col, uint32 value);
merror  mSet(Mat* mat, int row, int col, float value);
uint32	mGetu(const Mat* mat, int row, int col);
float	mGet(const Mat* mat, int row, int col);

// FG: Basic Opperations
merror 	mZero( Mat* mat );
merror 	mSetIdentity( Mat* mat);
merror 	mSetAll( Mat* mat, float value);
merror  mScale(Mat* mat, float scale);
merror	mCopy( const Mat* src, Mat* dst );
merror	mTranspose( const Mat* src, Mat* dst );
merror	mReshape(Mat* mat, int new_rows, int new_cols);
merror  mMinMaxLoc( const Mat* mat, int index, int dimension, float* min_val, float* max_val, int* min_loc, int* max_loc);
merror	mCountNonZero( const Mat *mat, int index, int dimension, uint32* count);

// FG: Basic Algebra
float   mTrace( const Mat *mat );
merror	mSum(const Mat *mat, int index, int dimension, float* sum);
merror 	mMean(const Mat *mat, int index, int dimension, float* mean);
//todo	mMode();
//todo	mMedian(const Mat *mat, int index, int dimension, float* median, float* min_val, float* max_val );
merror	mStdev( const Mat *mat, int index, int dimension, float* sum, float* mean, float* std );
merror	mMul(Mat* src1, Mat* src2, Mat* dst);
merror  mMulSub(Mat* src1, Mat* src2, Mat* src3, Mat* dst); // dst = src1 * src2 - src3
merror	mGEMM(Mat* src1, Mat* src2, float alpha, Mat* dst , uint32 flags );
merror	mElem( const Mat* src1, const Mat* src2, Mat* dst, const float scaler, const uint32 bit_vector, int op);

// FG: Linear Algebra
merror	mSVD(Mat* A, Mat* UT, Mat* W, Mat* VT);
//TODO	mEigenVV();
//TODO	mSolve();
//TODO	mInvert();
//TODO	mDet();
//TODO	mNormalize();
//TODO	mDot();
//TODO	mCross();

#endif
