open FC__Base;

let component = ReasonReact.statelessComponent(__MODULE__);

module IconQuestionmark = {
  let component = ReasonReact.statelessComponent(__MODULE__);
  // Adapted to pagecard title
  let iconStyle =
    Css.(
      style([
        height(`px(21)),
        width(`px(21)),
        paddingLeft(`em(0.6)),
        verticalAlign(`bottom),
        margin2(~v=`px(-2), ~h=`zero),
        cursor(`default),
      ])
    );
  // Local icon style
  let questionMarkstyle =
    Css.(
      style([
        selector("text", [fontSize(`em(0.7)), SVG.fill(`hex("989898"))]),
        selector("circle", [SVG.stroke(`hex("ccc"))]),
      ])
    );
  let make = _children => {
    ...component,
    render: _self => {
      <svg
        className={Css.merge([iconStyle, questionMarkstyle])}
        viewBox="0 0 20 20">
        <text x="10" y="13" fontWeight="bold" textAnchor="middle">
          {React.string("?")}
        </text>
        <circle cx="10" cy="10" r="8.5" strokeWidth="1" fill="none" />
      </svg>;
    },
  };
};

module PageCard = FC__PageCard;

let make = (~cdf: FC__Types.Dist.t, _children) => {
  ...component,
  render: _self => {
    <PageCard>
      <PageCard.HeaderRow>
        <PageCard.HeaderRow.Title>
          "New Prediction"->React.string
          <IconQuestionmark />
        </PageCard.HeaderRow.Title>
      </PageCard.HeaderRow>
      <PageCard.Section flex=true padding=false>
        <FC__Tab isActive=true flex=true> "Simple"->React.string </FC__Tab>
        <FC__Tab isActive=false flex=true> "Free-form"->React.string </FC__Tab>
        <FC__Tab isActive=false flex=true> "Custom"->React.string </FC__Tab>
      </PageCard.Section>
      <PageCard.Section grey=true borderBottom=true>
        <FC__CdfChart__Large cdf width=None />
      </PageCard.Section>
      <PageCard.Section grey=true borderBottom=true>
        <InputText fullWidth=true placeholder="5 to 50" />
      </PageCard.Section>
      <PageCard.Section>
        <InputHeader> "Comment"->React.string </InputHeader>
        <Textarea fullWidth=true />
        <PageCard.Spacer />
        <Button variant=FC__Button.Primary fullWidth=true>
          "Submit Prediction"->React.string
        </Button>
      </PageCard.Section>
    </PageCard>;
  },
};
