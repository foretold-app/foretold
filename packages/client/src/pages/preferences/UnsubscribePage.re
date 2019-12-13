[@react.component]
let make = (~loggedUser: Types.user) => {
  let agentId = Primary.Agent.AgentId.make(loggedUser.agentId);
  let (mutate, result, _) = Unsubscribe.Mutation.use();

  <SLayout head={SLayout.Header.textDiv("Unsubscribe")}>
    <FC.PageCard.BodyPadding>
      {switch (result) {
       | Error(_error) => <p> {"Something went wrong..." |> Utils.ste} </p>
       | Data(_) => <p> {"You are unsubscribed." |> Utils.ste} </p>
       | Loading(_) => <Spin />
       | _ =>
         mutate(
           ~variables=Unsubscribe.Query.make(~agentId, ())##variables,
           ~refetchQueries=[|"user"|],
           (),
         )
         |> ignore;

         <p> {"Unsubscribing..." |> Utils.ste} </p>;
       }}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};