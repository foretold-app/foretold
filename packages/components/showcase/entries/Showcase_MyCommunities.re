let makeItem = (name, icon): MyCommunities.item => {
  name,
  icon,
  href: "",
  onClick: _ => (),
};

let backgroundBox =
  Css.(style([background(`hex("ccc")), padding(`em(3.))]));

let myCommunities = () =>
  <div className=backgroundBox>
    <MyCommunities>
      <MyCommunities.Header name="FEEDS" />
      <MyCommunities.Item item={makeItem("Home", "HOME")} />
      <MyCommunities.Item item={makeItem("All Communities", "LOCK")} />
      <MyCommunities.Header name="MY COMMUNITIES" />
      <MyCommunities.Item
        item={makeItem("Slate-Star-Codex 2019", "PEOPLE")}
      />
      <MyCommunities.Item item={makeItem("AI Questions", "PEOPLE")} />
      <MyCommunities.Item item={makeItem("Other AI Questions", "PEOPLE")} />
      <MyCommunities.Item
        item={makeItem(
          "My Secret and Very Very Very Very Long-named Community",
          "PACMAN",
        )}
      />
    </MyCommunities>
  </div>;

let entry = EntryTypes.(entry(~title="MyCommunities", ~render=myCommunities));