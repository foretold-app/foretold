[@react.component]
let make = () => {
  <SLayout container=`fluid>
    <Center>
      <Icon icon="PACMAN" size="2em" />
      <p />
      {Lang.pageIsNotFound |> Utils.ste}
    </Center>
  </SLayout>;
};