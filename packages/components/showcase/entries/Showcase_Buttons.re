open FC;

let render = () =>
  <>
    <div> "Secondary"->React.string </div>
    <div>
      <Button.Jsx2 size=MediumShort>
        "Small Button"->React.string
      </Button.Jsx2>
      <Button.Jsx2 size=Medium> "Medium Button"->React.string </Button.Jsx2>
      <Button.Jsx2 size=Large> "Large Button"->React.string </Button.Jsx2>
    </div>
    <div> "Primary"->React.string </div>
    <div>
      <Button.Jsx2 size=MediumShort variant=Primary>
        "Small Button"->React.string
      </Button.Jsx2>
      <Button.Jsx2 size=Medium variant=Primary>
        "Medium Button"->React.string
      </Button.Jsx2>
      <Button.Jsx2 size=Large variant=Primary>
        "Large Button"->React.string
      </Button.Jsx2>
    </div>
  </>;

let entry = EntryTypes.(entry(~title="Buttons", ~render));
