[@react.component]
let make = (~loggedUser: Types.user) => {
  <SLayout head={<SLayout.TextDiv text="Make a New Bot" />}>
    <BotForm.Create loggedUser />
  </SLayout>;
};