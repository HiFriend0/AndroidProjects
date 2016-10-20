
#include <AR/gsub_es.h>
#include <Eden/glm.h>
#include <jni.h>
#include <ARWrapper/ARToolKitWrapperExportedAPI.h>
#include <unistd.h> 
#include <android/log.h>


#define JNIFUNCTION_DEMO(sig) Java_org_artoolkit_ar_samples_ARSimpleNativeCars_SimpleNativeRenderer_##sig

extern "C" {
JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoInitialise(JNIEnv * env, jobject  object)) ;
JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoShutdown(JNIEnv * env, jobject object)) ;
JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoSurfaceCreated(JNIEnv * env, jobject
                         object)) ;
JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoSurfaceChanged(JNIEnv * env, jobject
                         object, jint
                         w, jint
                         h)) ;
JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoDrawFrame(JNIEnv * env, jobject
                         obj)) ;
};

typedef struct ARModel {
    int patternID;
    ARdouble transformationMatrix[16];
    bool visible;
    GLMmodel *obj;
} ARModel;

#define NUM_MODELS 2
static ARModel models[NUM_MODELS] = {0};

static float lightAmbient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
static float lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static float lightPosition[4] = {0.0f, 0.0f, 1.0f, 0.0f};

JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoInitialise(JNIEnv * env, jobject
                         object)) {


const char *model0file = "Data/models/Porshe_911_GT3.obj";
const char *model1file = "Data/models/Ferrari_Modena_Spider.obj";

models[0].patternID = arwAddMarker("single;Data/hiro.patt;80");
arwSetMarkerOptionBool(models[0].patternID, ARW_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION, false);
arwSetMarkerOptionBool(models[0].patternID, ARW_MARKER_OPTION_FILTERED, true);

models[0].obj = glmReadOBJ2(model0file, 0, 0); // context 0, don't read textures yet.
if (!models[0].obj) {
LOGE("Error loading model from file '%s'.", model0file);
exit(-1);
}
glmScale(models[0]
.obj, 0.035f);

glmCreateArrays(models[0]
.obj, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
models[0].
    visible = false;

models[1].
patternID = arwAddMarker("single;Data/kanji.patt;80");
arwSetMarkerOptionBool(models[1]
.patternID, ARW_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION, false);
arwSetMarkerOptionBool(models[1]
.patternID, ARW_MARKER_OPTION_FILTERED, true);

models[1].
obj = glmReadOBJ2(model1file, 0, 0); // context 0, don't read textures yet.
if (!models[1].obj) {
LOGE("Error loading model from file '%s'.", model1file);
exit(-1);
}
glmScale(models[1]
.obj, 0.635f);

glmCreateArrays(models[1]
.obj, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
models[1].
visible = false;
}

JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoShutdown(JNIEnv * env, jobject
                         object)) {
}

JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoSurfaceCreated(JNIEnv * env, jobject
                         object)) {
glStateCacheFlush(); 
for (
int i = 0;
i < NUM_MODELS; i++) {
if (models[i].obj) {
glmDelete(models[i]
.obj, 0);
models[i].
obj = NULL;
}
}
}

JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoSurfaceChanged(JNIEnv * env, jobject
                         object, jint
                         w, jint
                         h)) {


JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoDrawFrame(JNIEnv * env, jobject
                         obj)) {

glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
glClear(GL_COLOR_BUFFER_BIT
| GL_DEPTH_BUFFER_BIT);


float proj[16];
arwGetProjectionMatrix(proj);
glMatrixMode(GL_PROJECTION);
glLoadMatrixf(proj);
glMatrixMode(GL_MODELVIEW);

glStateCacheEnableDepthTest();

glStateCacheEnableLighting();

glEnable(GL_LIGHT0);

for (
int i = 0;
i < NUM_MODELS; i++) {
models[i].
visible = arwQueryMarkerTransformation(models[i].patternID, models[i].transformationMatrix);

if (models[i].visible) {
glLoadMatrixf(models[i]
.transformationMatrix);

glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient
);
glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse
);
glLightfv(GL_LIGHT0, GL_POSITION, lightPosition
);

glmDrawArrays(models[i]
.obj, 0);
}

}

}
