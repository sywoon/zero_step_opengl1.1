#pragma once


#define SAVE_DELETE(p) { if (p) {delete p; p = NULL;} }
#define SAVE_DELETE_ARRAY(p) { if (p) {delete[] p; p = NULL;} }
#define SAVE_RELEASE(p) { if (p) {release(p); p = NULL;} }
#define SAVE_FREE(p) { if (p) {free(p); p = NULL;} }


#define g_app (*Application::getInstance())

