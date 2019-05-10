let make =
  <GroupHeader hero={"US Politics" |> ReasonReact.string}>
    <Div float=`left>
      <div
        className=Css.(
          style([fontSize(`px(24)), color(GroupHeader.Colors'.primary)])
        )>
        {"Great Group" |> ReasonReact.string}
      </div>
      <p
        className=Css.(
          style([color(GroupHeader.Colors'.secondary), marginTop(`px(0))])
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
  </GroupHeader>;