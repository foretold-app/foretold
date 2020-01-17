let display = id => {
  <Providers.AppContext.Consumer>
    ...{context => {
      module Config = {
        let entityGraph =
          context.globalSetting
          |> E.O.fmap((e: Types.globalSetting) => e.entityGraph)
          |> E.O.default(None);
      };
      module Ken = KenTools.Functor(Config);
      Ken.subjectIdToDisplay(id);
    }}
  </Providers.AppContext.Consumer>;
};

[@react.component]
let make = (~pageParams: Types.pageParams) => {
  <SLayout head={<SLayout.TextDiv text={pageParams.id} />}>
    <FC.PageCard.BodyPadding>
      {display(pageParams.id)}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};