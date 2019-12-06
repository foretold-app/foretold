[@bs.config {jsx: 3}];

open Rationale.Function.Infix;

module CMutationForm =
  MutationForm.Make({
    type queryType = ChannelCreate.Query.t;
  });

[@react.component]
let make = () => {
  let form = mutation =>
    ChannelForm.Form.use(
      ~onSubmit=
        values => {
          ChannelCreate.mutate(
            mutation,
            values.state.values.name,
            values.state.values.description,
            values.state.values.isPublic == "TRUE" ? true : false,
            values.state.values.isArchived == "TRUE" ? true : false,
          );
          None;
        },
      ~initialState={
        name: "",
        description: "",
        isPublic: "TRUE",
        isArchived: "FALSE",
      },
      ~schema=ChannelForm.Form.Validation.Schema([||]),
      (),
    );

  let body =
    <FC.PageCard.BodyPadding>
      <ChannelCreate.Mutation>
        ...{(mutation, data) => {
          let reform = form(mutation);

          <ChannelForm.Form.Provider value=reform>
            {CMutationForm.showWithLoading2(
               ~result=data.result,
               ~form=ChannelForm.showForm(false, reform),
               ~onSuccess=
                 (response: ChannelCreate.Query.t) => {
                   switch (response##channelCreate) {
                   | Some(channel) =>
                     Routing.Url.push(ChannelShow(channel##id))
                   | _ => ()
                   };
                   <Null />;
                 },
               (),
             )}
          </ChannelForm.Form.Provider>;
        }}
      </ChannelCreate.Mutation>
    </FC.PageCard.BodyPadding>;

  <SLayout head={SLayout.Header.textDiv("Create a New Community")}>
    body
  </SLayout>;
};