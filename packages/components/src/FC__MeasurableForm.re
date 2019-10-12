open FC__Base;

module PageCard = FC__PageCard;
module Tab = FC__Tab;
module TabList = FC__TabList;

// Could this be in some flexrow or part of Div.Jsx2 component?
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
          <FC__HelpDropdown />
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
           <Div.Jsx2 flexDirection=`row styles=[flexRowContainer]>
             <Div.Jsx2 flex={`num(1.0)} styles=[flexRowItem]>
               <InputLabel> "Min"->React.string </InputLabel>
               <TextInput fullWidth=true />
             </Div.Jsx2>
             <Div.Jsx2 flex={`num(1.0)} styles=[flexRowItem]>
               <InputLabel> "Max"->React.string </InputLabel>
               <TextInput fullWidth=true />
             </Div.Jsx2>
             <Div.Jsx2 styles=[flexRowItem]>
               <Button variant=Button.Secondary>
                 "Clear"->React.string
               </Button>
             </Div.Jsx2>
           </Div.Jsx2>
         | FreeformTab => <TextInput fullWidth=true placeholder="5 to 50" />
         | CustomTab =>
           <div>
             <div>
               <DropdownSelect
                 initialValue={Some("CDF")}
                 values=[("CDF", "CDF"), ("PDF", "PDF")]
               />
               <FC__HelpDropdown />
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
        <Button variant=Button.Primary fullWidth=true size=Large>
          "Submit Prediction"->React.string
        </Button>
      </PageCard.Section>
    </PageCard>,
};