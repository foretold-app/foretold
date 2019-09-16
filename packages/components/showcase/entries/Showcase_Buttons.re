open FC;

let render = () =>
  <>
    <div> "Secondary"->React.string </div>
    <div>
      <Button size=Small> "Small Button"->React.string </Button>
      <Button size=Median> "Median Button"->React.string </Button>
      <Button size=Large> "Large Button"->React.string </Button>
    </div>
    <div> "Primary"->React.string </div>
    <div>
      <Button size=Small variant=Primary>
        "Small Button"->React.string
      </Button>
      <Button size=Median variant=Primary>
        "Median Button"->React.string
      </Button>
      <Button size=Large variant=Primary>
        "Large Button"->React.string
      </Button>
    </div>
  </>;

let entry = EntryTypes.(entry(~title="Buttons", ~render));