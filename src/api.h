#pragma once

typedef struct
{
    char *appname;
    char *appversion;
    char *appid;
} AppMetadata;

typedef struct
{
    char *url;
    char *creator;
    char *copyright;
    char *type;
} AppMetadataProperties;

extern AppMetadata app_metadata;
extern AppMetadataProperties app_metadata_props;

void init_api();
void release_api();
