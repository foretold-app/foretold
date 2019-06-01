open FC__Base;

let component = ReasonReact.statelessComponent(__MODULE__);

module PageCard = FC__PageCard;
module Tab = FC__Tab;

let make = (~cdf: FC__Types.Dist.t, _children) => {
  ...component,
  render: _self =>
    <PageCard>
      <PageCard.HeaderRow>
        <PageCard.HeaderRow.Title>
          "New Prediction"->React.string
          <Icon.Questionmark />
        </PageCard.HeaderRow.Title>
      </PageCard.HeaderRow>
      <PageCard.Section flex=true padding=false>
        <Tab isActive=true flex=true> "Simple"->React.string </Tab>
        <Tab flex=true> "Free-form"->React.string </Tab>
        <Tab flex=true> "Custom"->React.string </Tab>
      </PageCard.Section>
      <PageCard.Section grey=true borderBottom=true padding=false>
        <FC__CdfChart__Large cdf width=None />
      </PageCard.Section>
      <PageCard.Section grey=true>
        <TextInput fullWidth=true placeholder="5 to 50" />
      </PageCard.Section>
      <PageCard.Section>
        <InputLabel> "Comment"->React.string </InputLabel>
        <TextArea fullWidth=true />
        <PageCard.Spacer />
        <Button
          variant=Button.Primary fullWidth=true verticalPadding={`px(10)}>
          "Submit Prediction"->React.string
        </Button>
      </PageCard.Section>
    </PageCard>,
};