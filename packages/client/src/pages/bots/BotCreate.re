[@react.component]
let make = (~loggedUser: Types.user) => {
  <SLayout head={SLayout.Header.textDiv("Make a New Bot")}>
    <BotForm.Create loggedUser />
  </SLayout>;
};