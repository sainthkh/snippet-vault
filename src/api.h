#pragma once

#include "stringstore.h"

typedef struct
{
    string_id appname;
    string_id appversion;
    string_id appid;
} AppMetadata;

typedef struct
{
    string_id url;
    string_id creator;
    string_id copyright;
    string_id type;
} AppMetadataProperties;

typedef struct
{
    string_id name;
    int width;
    int height;
} AppWindow;

extern AppWindow app_window;
extern AppMetadata app_metadata;
extern AppMetadataProperties app_metadata_props;

void api_init();
void api_release();
