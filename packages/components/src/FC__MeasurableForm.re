open FC__Base;

module PageCard = FC__PageCard;
module Tab = FC__Tab;

module TabList = {
  let component = ReasonReact.statelessComponent(__MODULE__);
  let make = (~selected, ~flex=false, ~onClick=?, ~list, _children) => {
    ...component,
    render: _self => {
      list
      |> E.L.React.map(((key, label)) =>
           <Tab
             key
             onClick={e =>
               switch (onClick) {
               | Some(onClick) =>
                 e->ReactEvent.Synthetic.preventDefault;
                 onClick(key);
               | None => ()
               }
             }
             isActive={selected == key}
             flex>
             label->React.string
           </Tab>
         );
    },
  };
};

type state = {tabSelected: string};

type action =
  | ChangeTab(string);

let component = ReasonReact.reducerComponent(__MODULE__);
let make = (~cdf: FC__Types.Dist.t, _children) => {
  ...component,
  initialState: () => {tabSelected: "simple"},
  reducer: (action: action, _state: state) => {
    switch (action) {
    | ChangeTab(tabKey) => ReasonReact.Update({tabSelected: tabKey})
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
      <PageCard.Section flex=true padding=false>
        <TabList
          selected={self.state.tabSelected}
          onClick={key => self.send(ChangeTab(key))}
          flex=true
          list=[
            ("simple", "Simple2"),
            ("freeForm", "Free-form"),
            ("custom", "Custom"),
          ]
        />
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
        <PageCard.VerticalSpace />
        <Button
          variant=Button.Primary fullWidth=true verticalPadding={`px(10)}>
          "Submit Prediction"->React.string
        </Button>
      </PageCard.Section>
    </PageCard>,
};
