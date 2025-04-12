declare function print(str: string): void;

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

type Scene = {
  init: () => void;
  update: () => void;
  render: () => void;
  release: () => void;
}

type SceneId = number;

declare function api_addScene(scene: Scene): SceneId;

