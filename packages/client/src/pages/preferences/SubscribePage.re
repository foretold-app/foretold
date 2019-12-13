[@react.component]
let make = (~loggedUser: Types.user) => {
  let agentId = Primary.Agent.AgentId.make(loggedUser.agentId);
  let (mutate, result, _) = Subscribe.Mutation.use();

  <SLayout head={SLayout.Header.textDiv("Subscribe")}>
    <FC.PageCard.BodyPadding>
      {switch (result) {
       | Error(_error) => <p> {"Something went wrong..." |> Utils.ste} </p>
       | Data(_) => <p> {"You are subscribed." |> Utils.ste} </p>
       | Loading(_) => <Spin />
       | _ =>
         mutate(
           ~variables=Subscribe.Query.make(~agentId, ())##variables,
           ~refetchQueries=[|"user"|],
           (),
         )
         |> ignore;

         <p> {"Subscribing..." |> Utils.ste} </p>;
       }}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};