open FC.Base;

let entries =
  EntryTypes.[
    folder(
      ~title="Link",
      ~children=[
        entry(~title="Link1b", ~render=() =>
          <Link> "Test link"->React.string </Link>
        ),
        entry(~title="Link2", ~render=() =>
          <Link> "Test link2"->React.string </Link>
        ),
        entry(~title="Link3", ~render=() =>
          <Link> "Test link3"->React.string </Link>
        ),
      ],
    ) ,
  ] @ Showcase_Dropdown.entries;