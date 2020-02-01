let makeItem = (name, icon): MyCommunities.item => {
  name,
  icon,
  href: "",
  onClick: _ => (),
};

let backgroundBox =
  Css.(style([background(`hex("ccc")), padding(`em(3.))]));

let innerBox =
  Css.(
    style([
      fontSize(`rem(1.1)),
      width(`em(20.)),
      border(`px(1), `solid, `hex("d5d2d2")),
      padding2(~v=`em(0.5), ~h=`em(0.)),
      borderRadius(`px(5)),
      background(`hex("fff")),
    ])
  );

let myCommunities = () =>
  <div className=backgroundBox>
    <div className=innerBox>
      <MyCommunities>
        <MyCommunities.Header name="FEEDS" />
        <MyCommunities.Item item={makeItem("Home", "HOME")} />
        <MyCommunities.Item item={makeItem("All Communities", "LIST")} />
        <MyCommunities.Header name="MY COMMUNITIES" />
        <MyCommunities.Item
          item={makeItem("Slate-Star-Codex 2019", "PEOPLE")}
        />
        <MyCommunities.Item item={makeItem("AI Questions", "PEOPLE")} />
        <MyCommunities.Item item={makeItem("Other AI Questions", "LOCK")} />
        <MyCommunities.Item
          item={makeItem(
            "My Secret and Very Very Very Very Long-named Community",
            "LOCK",
          )}
        />
        <MyCommunities.Header name="OPTIONS" />
        <MyCommunities.Item
          item={makeItem("Create a New Community", "CIRCLE_PLUS")}
        />
      </MyCommunities>
    </div>
  </div>;

let entry = EntryTypes.(entry(~title="MyCommunities", ~render=myCommunities));