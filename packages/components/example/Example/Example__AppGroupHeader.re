open FC;
open Base;

let make =
  <Div.Jsx2>
    <GroupHeader>
      <Div.Jsx2 float=`left>
        <div
          className=Css.(
            style([
              fontSize(`em(1.4)),
              marginTop(`px(5)),
              Colors.FontWeights.heavy,
              color(Colors.darkLink),
            ])
          )>
          {"Great Community" |> ReasonReact.string}
        </div>
        <p
          className=Css.(
            style([
              color(`hex("36485c")),
              marginTop(`px(5)),
              marginBottom(`px(3)),
            ])
          )>
          {"Lots of description for the group would be here"
           |> ReasonReact.string}
        </p>
      </Div.Jsx2>
      <Div.Jsx2 float=`right>
        {GroupHeader.actionButton(
           ~variant=Secondary,
           [|"Leave Channel" |> ReasonReact.string|],
         )
         |> ReasonReact.element}
        {GroupHeader.actionButton([|"Create Question" |> ReasonReact.string|])
         |> ReasonReact.element}
      </Div.Jsx2>
    </GroupHeader>
    <GroupHeader.SubHeader>
      <Tab isActive=true> {"Questions" |> ReasonReact.string} </Tab>
      <Tab isActive=false> {"Knowledge Graph" |> ReasonReact.string} </Tab>
      <Tab isActive=false> {"Leaderboard" |> ReasonReact.string} </Tab>
      <Tab isActive=false> {"Settings" |> ReasonReact.string} </Tab>
    </GroupHeader.SubHeader>
  </Div.Jsx2>;