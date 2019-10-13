open FC;

let content1: FC__HelpDropdown.content = {
  headerContent: "Log Scoring",
  bodyContent: "Log Scoring is simple. $$x^2$$",
};

let render = () =>
  <span>
    <FC__HelpDropdown content=content1 />
    <FC__HelpDropdown.Inside content=content1 />
  </span>;

let entry = EntryTypes.(entry(~title="HelpDropDown", ~render));