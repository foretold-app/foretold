open FC.Base;

let entries =
  EntryTypes.[
    Showcase_Buttons.entry,
    Showcase_PageCard.entry,
    Showcase_NumberShower.entry,
    Showcase_MeasurableForm.entry,
    Showcase_Colors.entry,
    Scoring.entry,
    Showcase_AgentLink.entry,
    Showcase_HelpDropdown.entry,
    Showcase_Alerts.entry,
    folder(
      ~title="Link",
      ~children=[
        entry(~title="Link1b", ~render=() =>
          <FC__Link.Jsx2> "Test link"->React.string </FC__Link.Jsx2>
        ),
        entry(~title="Link2", ~render=() =>
          <FC__Link.Jsx2> "Test link2"->React.string </FC__Link.Jsx2>
        ),
        entry(~title="Link3", ~render=() =>
          <FC__Link.Jsx2> "Test link3"->React.string </FC__Link.Jsx2>
        ),
      ],
    ),
  ]
  @ Showcase_Dropdown.entries
  @ Showcase_Menu.entries
  @ Showcase_DropdownMenu.entries
  @ Showcase_DropdownSelect.entries;