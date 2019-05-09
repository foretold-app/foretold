open BsStorybook.Story;

let _module = [%bs.raw "module"];

storiesOf("My First Reason Story", _module)
->(
    add("first chapter", () =>
      <span> {ReasonReact.string("Hello bs-storybook!")} </span>
    )
  );
/* |. add("firstt chapter", () => <span> <CdfInput /> </span>); */