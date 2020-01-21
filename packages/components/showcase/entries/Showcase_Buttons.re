open FC;
open Button;

let render = () =>
  <>
    <div> "Secondary"->React.string </div>
    <div>
      <Button size=MediumShort> "Small Button"->React.string </Button>
      <Button size=Medium> "Medium Button"->React.string </Button>
      <Button size=Large> "Large Button"->React.string </Button>
    </div>
    <div> "Primary"->React.string </div>
    <div>
      <Button size=MediumShort variant=Primary>
        "Small Button"->React.string
      </Button>
      <Button size=Medium variant=Primary>
        "Medium Button"->React.string
      </Button>
      <Button size=Large variant=Primary>
        "Large Button"->React.string
      </Button>
    </div>
  </>;

let entry = EntryTypes.(entry(~title="Buttons", ~render));
