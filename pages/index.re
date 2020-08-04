%raw
"require('../styles/main.css')";

open Util.ReactStuff;

module P = {
  [@react.component]
  let make = (~children) => <p className="mb-2"> children </p>;
};

type message = {
  body: string,
  color: option(string),
};
type state = {
  dummy: bool,
  messages: list(message),
};

module TwitchOverlay = {
  [@react.component]
  let make = (~channelName) => {
    let (state, setState) =
      React.useState(() => {{dummy: false, messages: []}});

    React.useEffect0(() => {
      let client =
        Tmi.makeClient({
          connection: {
            secure: true,
            reconnect: true,
          },
          channels: [|channelName|],
        });

      Tmi.connect(client);

      let messageHandler: Tmi.onHandler =
        (channel, tags, message, _self) => {
          let speaker = tags.displayName;

          let payload = {
            body: {j|$speaker: $message|j},
            color: tags.color->Js.Nullable.toOption,
          };
          Js.log4("Incoming message: ", channel, tags, message);
          Js.log(payload);
          setState(oldState => {
            {
              ...oldState,
              messages:
                [payload, ...oldState.messages]
                ->Belt.List.keepWithIndexU((. _, idx) => idx < 50),
            }
          });
        };

      Tmi.on(client, "message", messageHandler);
      None;
    });

    <div className="flex md:flex-row-reverse flex-wrap w-screen h-screen">
      <div
        style={ReactDOMRe.Style.make(~height="75%", ())}
        className="w-full md:w-1/6 bg-gray-400 p-4 text-left text-gray-700 text-xs">
        {j|$channelName chat|j}->s
        <ul className="flex flex-col-reverse">
          {state.messages
           ->Belt.List.map(message => {
               <li
                 key={message.body}
                 style={ReactDOMRe.Style.make(~color=?message.color, ())}>
                 message.body->s
               </li>
             })
           ->Belt.List.toArray
           ->React.array}
        </ul>
      </div>
      <div
        className="flex w-full bg-gray-500 text-center text-gray-200"
        style={ReactDOMRe.Style.make(~height="25%", ())}>
        <div className="w-3/4 bg-gray-400 text-center text-gray-700">
          "progress"->s
          <Issues.Container />
        </div>
        <div className="w-1/4 bg-gray-900 text-center text-gray-200">
          "Camera"->s
        </div>
      </div>
    </div>;
  };
};

[@react.component]
let make = () => {
  <TwitchOverlay channelName="seangrove_io" />;
};

let default = make;
