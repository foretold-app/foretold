open FC;

let content1: FC__HelpDropdown.content = {
  headerContent: "Log Scoring",
  bodyContent: "Log Scoring is special. Here's latex: $$ e^3$$",
};

let render = () =>
  <div>
    <div> <FC__HelpDropdown.Overlay content=content1 /> </div>
    <div> <FC__HelpDropdown content=content1 /> </div>
  </div>;

let entry = EntryTypes.(entry(~title="HelpDropDown", ~render));