open FC__Base;

module PageCard = FC__PageCard;
module Tab = FC__Tab;
module TabList = FC__TabList;

// Could this be in some flexrow or part of Div component?
let flexRowContainer =
  Css.(style([margin2(~v=`zero, ~h=`px(-6)), alignItems(`flexEnd)]));
let flexRowItem = Css.(style([margin2(~v=`zero, ~h=`px(6))]));

type tabs =
  | SimpleTab
  | FreeformTab
  | CustomTab;

type state = {selectedTab: tabs};

type action =
  | ChangeTab(tabs);

let component = ReasonReact.reducerComponent(__MODULE__);
let make = (~cdf: FC__Types.Dist.t, _children) => {
  ...component,
  initialState: () => {selectedTab: SimpleTab},
  reducer: (action: action, _state: state) => {
    switch (action) {
    | ChangeTab(tab) => ReasonReact.Update({selectedTab: tab})
    };
  },
  render: self =>
    <PageCard>
      <PageCard.HeaderRow>
        <PageCard.HeaderRow.Title>
          "New Prediction"->React.string
          <Icon.Questionmark />
        </PageCard.HeaderRow.Title>
      </PageCard.HeaderRow>
      <PageCard.Section flex=true padding=`none>
        <TabList
          selected={self.state.selectedTab}
          onClick={key => self.send(ChangeTab(key))}
          list=[
            (SimpleTab, "Simple"),
            (FreeformTab, "Free-form"),
            (CustomTab, "Custom"),
          ]
          flex=true
        />
      </PageCard.Section>
      <PageCard.Section background=`grey border=`bottom padding=`top>
        <FC__CdfChart__Large cdf width=None />
      </PageCard.Section>
      <PageCard.Section background=`grey>
        {switch (self.state.selectedTab) {
         | SimpleTab =>
           <Div flexDirection=`row styles=[flexRowContainer]>
             <Div flex={`num(1.)} styles=[flexRowItem]>
               <InputLabel> "Min"->React.string </InputLabel>
               <TextInput fullWidth=true />
             </Div>
             <Div flex={`num(1.)} styles=[flexRowItem]>
               <InputLabel> "Max"->React.string </InputLabel>
               <TextInput fullWidth=true />
             </Div>
             <Div styles=[flexRowItem]>
               <Button variant=Button.Secondary>
                 "Clear"->React.string
               </Button>
             </Div>
           </Div>
         | FreeformTab => <TextInput fullWidth=true placeholder="5 to 50" />
         | CustomTab =>
           <div>
             <div>
               <DropdownSelect
                 initialValue={Some("CDF")}
                 values=[("CDF", "CDF"), ("PDF", "PDF")]
               />
               <Icon.Questionmark />
             </div>
             <PageCard.VerticalSpace />
             <Alert type_=`error>
               "Input is not a valid PDF"->React.string
             </Alert>
             <PageCard.VerticalSpace />
             <TextArea rows=4 fullWidth=true />
           </div>
         }}
      </PageCard.Section>
      <PageCard.Section>
        <InputLabel> "Comment"->React.string </InputLabel>
        <TextArea fullWidth=true />
        <PageCard.VerticalSpace />
        <Button
          variant=Button.Primary fullWidth=true verticalPadding={`px(10)}>
          "Submit Prediction"->React.string
        </Button>
      </PageCard.Section>
    </PageCard>,
};