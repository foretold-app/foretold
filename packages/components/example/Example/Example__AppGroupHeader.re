open FC;
open Base;

let make =
  <Div>
    <GroupHeader>
      <Div float=`left>
        <div
          className=Css.(
            style([
              fontSize(`em(1.2)),
              marginTop(`px(5)),
              Colors.FontWeights.heavy,
              color(Colors.link),
            ])
          )>
          {"Great Group" |> ReasonReact.string}
        </div>
        <p
          className=Css.(
            style([
              color(FC__Colors.Text.LightBackground.p),
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
          GroupHeader.secondaryLink(
            ~href="#",
            [|"Leave Channel" |> ReasonReact.string|],
          )
          |> ReasonReact.element
        }
        {
          GroupHeader.link(
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