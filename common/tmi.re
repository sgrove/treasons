type connectionConfig = {
  secure: bool,
  reconnect: bool,
};

type clientConfig = {
  connection: connectionConfig,
  channels: array(string),
};

type userType = [
  /* | [@bs.as ""] `blank */
  | `mods
  | `global_mod
  | `admin
  | `staff
];

type commonUserState = {
  /* badges?: Badges; */
  color: Js.Nullable.t(string),
  [@bs.as "display-name"]
  displayName: Js.Nullable.t(string),
  /*  emotes?: { [emoteid: string]: string[] }, */
  id: Js.Nullable.t(string),
  [@bs.as "mod"]
  isMod: Js.Nullable.t(bool),
  turbo: Js.Nullable.t(bool),
  [@bs.as "emotes-raw"]
  emotesRaw: Js.Nullable.t(string),
  [@bs.as "badges-raw"]
  badgesRaw: Js.Nullable.t(string),
  [@bs.as "room-id"]
  roomId: Js.Nullable.t(string),
  subscriber: Js.Nullable.t(bool),
  [@bs.as "user-type"]
  userType: Js.Nullable.t(userType),
  [@bs.as "user-id"]
  userId: Js.Nullable.t(string),
  [@bs.as "tmi-sent-ts"]
  tmiSentTs: Js.Nullable.t(string),
  flags: Js.Nullable.t(string),
  /* [paramater: string]: any, */
};

type client;
type channel;

[@bs.module "tmi.js"] [@bs.new]
external makeClient: clientConfig => client = "Client";

[@bs.send] external connect: client => unit = "connect";

type onHandler = (channel, commonUserState, string, bool) => unit;

[@bs.send] external on: (client, string, onHandler) => unit = "on";

/* const client = new tmi.Client({ */
/* 	connection: { */
/* 		secure: true, */
/* 		reconnect: true */
/* 	}, */
/* 	channels: [ 'my-name' ] */
/* }); */

/* client.connect(); */

/* client.on('message', (channel, tags, message, self) => { */
/* 	// "Alca: Hello, World!" */
/* 	console.log(`${tags['display-name']}: ${message}`); */
/* }); */
