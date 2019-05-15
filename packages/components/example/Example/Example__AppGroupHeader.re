open FC;
open Base;

let make =
  <Div>
    <GroupHeader hero={"US Politics" |> ReasonReact.string}>
      <Div float=`left>
        <div
          className=Css.(
            style([
              fontSize(`px(24)),
              Colors.FontWeights.heavy,
              color(GroupHeader.Colors'.primary),
            ])
          )>
          {"Great Group" |> ReasonReact.string}
        </div>
        <p
          className=Css.(
            style([
              color(GroupHeader.Colors'.secondary),
              marginTop(`px(0)),
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
      <Tab isActive=false> {"Questions" |> ReasonReact.string} </Tab>
      <Tab isActive=false> {"Questions" |> ReasonReact.string} </Tab>
      <Tab isActive=false> {"Questions" |> ReasonReact.string} </Tab>
    </GroupHeader.SubHeader>
  </Div>;