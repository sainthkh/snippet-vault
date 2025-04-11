type AppWindow = {
  name: string;
  width: number;
  height: number;
}

type AppMeta = {
  appname: string;
  appversion: string;
  appid: string;
}

type AppMetaProps = {
  url: string;
  creator: string;
  copyright: string;
  type: string;
}

declare function api_appInit(window: AppWindow, meta: AppMeta, metaProps: AppMetaProps): void;
