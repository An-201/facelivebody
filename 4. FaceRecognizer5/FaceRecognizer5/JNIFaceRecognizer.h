/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_seeta_sdk_FaceRecognizer */

#ifndef _Included_com_seeta_sdk_FaceRecognizer
#define _Included_com_seeta_sdk_FaceRecognizer
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    construct
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_seeta_sdk_FaceRecognizer_construct__Ljava_lang_String_2
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    construct
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_seeta_sdk_FaceRecognizer_construct__Ljava_lang_String_2Ljava_lang_String_2
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    dispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_seeta_sdk_FaceRecognizer_dispose
  (JNIEnv *, jobject);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    LoadModel
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_seeta_sdk_FaceRecognizer_LoadModel__Ljava_lang_String_2
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    LoadModel
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_seeta_sdk_FaceRecognizer_LoadModel__Ljava_lang_String_2Ljava_lang_String_2
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    GetFeatureSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_seeta_sdk_FaceRecognizer_GetFeatureSize
  (JNIEnv *, jobject);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    GetCropWidth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_seeta_sdk_FaceRecognizer_GetCropWidth
  (JNIEnv *, jobject);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    GetCropHeight
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_seeta_sdk_FaceRecognizer_GetCropHeight
  (JNIEnv *, jobject);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    GetCropChannels
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_seeta_sdk_FaceRecognizer_GetCropChannels
  (JNIEnv *, jobject);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    CropFace
 * Signature: (Lcom/seeta/sdk/ImageData;[Lcom/seeta/sdk/Point;Lcom/seeta/sdk/ImageData;I)Z
 */
JNIEXPORT jboolean JNICALL Java_com_seeta_sdk_FaceRecognizer_CropFace
  (JNIEnv *, jobject, jobject, jobjectArray, jobject, jint);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    ExtractFeature
 * Signature: (Lcom/seeta/sdk/ImageData;[F)Z
 */
JNIEXPORT jboolean JNICALL Java_com_seeta_sdk_FaceRecognizer_ExtractFeature__Lcom_seeta_sdk_ImageData_2_3F
  (JNIEnv *, jobject, jobject, jfloatArray);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    ExtractFeatureNormalized
 * Signature: (Lcom/seeta/sdk/ImageData;[F)Z
 */
JNIEXPORT jboolean JNICALL Java_com_seeta_sdk_FaceRecognizer_ExtractFeatureNormalized__Lcom_seeta_sdk_ImageData_2_3F
  (JNIEnv *, jobject, jobject, jfloatArray);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    ExtractFeatureWithCrop
 * Signature: (Lcom/seeta/sdk/ImageData;[Lcom/seeta/sdk/Point;[FI)Z
 */
JNIEXPORT jboolean JNICALL Java_com_seeta_sdk_FaceRecognizer_ExtractFeatureWithCrop__Lcom_seeta_sdk_ImageData_2_3Lcom_seeta_sdk_Point_2_3FI
  (JNIEnv *, jobject, jobject, jobjectArray, jfloatArray, jint);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    ExtractFeatureWithCropNormalized
 * Signature: (Lcom/seeta/sdk/ImageData;[Lcom/seeta/sdk/Point;[FI)Z
 */
JNIEXPORT jboolean JNICALL Java_com_seeta_sdk_FaceRecognizer_ExtractFeatureWithCropNormalized__Lcom_seeta_sdk_ImageData_2_3Lcom_seeta_sdk_Point_2_3FI
  (JNIEnv *, jobject, jobject, jobjectArray, jfloatArray, jint);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    CalcSimilarity
 * Signature: ([F[FJ)F
 */
JNIEXPORT jfloat JNICALL Java_com_seeta_sdk_FaceRecognizer_CalcSimilarity
  (JNIEnv *, jobject, jfloatArray, jfloatArray, jlong);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    CalcSimilarityNormalized
 * Signature: ([F[FJ)F
 */
JNIEXPORT jfloat JNICALL Java_com_seeta_sdk_FaceRecognizer_CalcSimilarityNormalized
  (JNIEnv *, jobject, jfloatArray, jfloatArray, jlong);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    SetNumThreads
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_seeta_sdk_FaceRecognizer_SetNumThreads
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    SetMaxBatchGlobal
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_seeta_sdk_FaceRecognizer_SetMaxBatchGlobal
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    GetMaxBatch
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_seeta_sdk_FaceRecognizer_GetMaxBatch
  (JNIEnv *, jobject);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    SetCoreNumberGlobal
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_seeta_sdk_FaceRecognizer_SetCoreNumberGlobal
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    GetCoreNumber
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_seeta_sdk_FaceRecognizer_GetCoreNumber
  (JNIEnv *, jobject);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    ExtractFeature
 * Signature: ([Lcom/seeta/sdk/ImageData;[F)Z
 */
JNIEXPORT jboolean JNICALL Java_com_seeta_sdk_FaceRecognizer_ExtractFeature___3Lcom_seeta_sdk_ImageData_2_3F
  (JNIEnv *, jobject, jobjectArray, jfloatArray);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    ExtractFeatureNormalized
 * Signature: ([Lcom/seeta/sdk/ImageData;[F)Z
 */
JNIEXPORT jboolean JNICALL Java_com_seeta_sdk_FaceRecognizer_ExtractFeatureNormalized___3Lcom_seeta_sdk_ImageData_2_3F
  (JNIEnv *, jobject, jobjectArray, jfloatArray);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    ExtractFeatureWithCrop
 * Signature: ([Lcom/seeta/sdk/ImageData;[Lcom/seeta/sdk/Point;[F)Z
 */
JNIEXPORT jboolean JNICALL Java_com_seeta_sdk_FaceRecognizer_ExtractFeatureWithCrop___3Lcom_seeta_sdk_ImageData_2_3Lcom_seeta_sdk_Point_2_3F
  (JNIEnv *, jobject, jobjectArray, jobjectArray, jfloatArray);

/*
 * Class:     com_seeta_sdk_FaceRecognizer
 * Method:    ExtractFeatureWithCropNormalized
 * Signature: ([Lcom/seeta/sdk/ImageData;[Lcom/seeta/sdk/Point;[F)Z
 */
JNIEXPORT jboolean JNICALL Java_com_seeta_sdk_FaceRecognizer_ExtractFeatureWithCropNormalized___3Lcom_seeta_sdk_ImageData_2_3Lcom_seeta_sdk_Point_2_3F
  (JNIEnv *, jobject, jobjectArray, jobjectArray, jfloatArray);

#ifdef __cplusplus
}
#endif
#endif
