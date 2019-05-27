open FC;
open Base;

let make =
  <Div>
    <GroupHeader>
      <Div float=`left>
        <div
          className=Css.(
            style([
              fontSize(`em(1.4)),
              marginTop(`px(5)),
              Colors.FontWeights.heavy,
              color(Colors.darkLink),
            ])
          )>
          {"Great Group" |> ReasonReact.string}
        </div>
        <p
          className=Css.(
            style([
              color(`hex("36485c")),
              marginTop(`px(5)),
              marginBottom(`px(3)),
            ])
          )>
          {
            "Lots of description for the group would be here"
            |> ReasonReact.string
          }
        </p>
      </Div>
      <Div float=`right>
        {
          GroupHeader.actionButton(
            ~variant=Secondary,
            ~href="#",
            [|"Leave Channel" |> ReasonReact.string|],
          )
          |> ReasonReact.element
        }
        {
          GroupHeader.actionButton(
            ~href="#",
            [|"Create Question" |> ReasonReact.string|],
          )
          |> ReasonReact.element
        }
      </Div>
    </GroupHeader>
    <GroupHeader.SubHeader>
      <Tab isActive=true> {"Questions" |> ReasonReact.string} </Tab>
      <Tab isActive=false> {"Knowledge Graph" |> ReasonReact.string} </Tab>
      <Tab isActive=false> {"Leaderboard" |> ReasonReact.string} </Tab>
      <Tab isActive=false> {"Settings" |> ReasonReact.string} </Tab>
    </GroupHeader.SubHeader>
  </Div>;