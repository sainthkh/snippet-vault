api_appInit({
  name: "Snake!!",
  width: 1280,
  height: 720,
}, {
  appname: "snake",
  appversion: "1.0",
  appid: "com.sainthkh.snake",
}, {
  url: "https://examples.libsdl.org/SDL3/demo/01-snake/",
  creator: "sainthkh",
  copyright: "MIT LICENSE",
  type: "game",
})

api_addScene({
  init: () => {
    print("init");
  },
  update: () => {
  },
  render: () => {
  },
  release: () => {
  },
})
