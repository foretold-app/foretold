open FC__Base;

let component = ReasonReact.statelessComponent(__MODULE__);

module Styles = {
  open Css;

  let cardTitle = style([padding2(~v=`em(1.2), ~h=`zero), margin(`zero)]);

  let chartContainer =
    style([
      backgroundColor(Colors.smokeWhite),
      clear(`both),
      borderBottom(`px(1), `solid, Colors.accentBlue1a),
    ]);

  let sectionPadding = padding2(~v=`em(1.2), ~h=`em(0.8));

  let inputSection =
    style([backgroundColor(Colors.smokeWhite), sectionPadding]);

  let commentSection = style([sectionPadding]);

  let inputHeader =
    style([
      fontSize(`px(14)),
      fontWeight(`bold),
      paddingLeft(`em(0.4)),
      paddingBottom(`em(0.6)),
    ]);

  let formInputInset = [
    border(`px(1), `solid, Colors.border),
    borderRadius(`px(4)),
    padding(`em(0.6)),
    boxShadows([`shadow("inset 0 1px 2px rgba(0,0,0.7,0.08)")]),
  ];

  let inputFullWidth = [width(`percent(100.)), boxSizing(`borderBox)];

  let textInputFullWidth = style(formInputInset @ inputFullWidth);

  let textAreaFullWidth = style(formInputInset @ inputFullWidth);

  let buttonFullWidth =
    style(
      inputFullWidth
      @ [
        backgroundColor(Colors.link),
        color(Colors.buttonBlueColor),
        borderStyle(`none),
        padding(`rem(0.8)),
        marginTop(`rem(0.8)),
        textAlign(`center),
        borderRadius(`px(5)),
        transitions([
          `transition("background 200ms ease-in-out"),
          `transition("transform 150ms ease"),
        ]),
        selector(
          ":hover, :focus",
          [backgroundColor(Colors.linkHover)],
        ),
        selector(":active", [transform(`scale((1., 0.96)))]),
      ],
    );
};

module IconQuestionmark = {
  let component = ReasonReact.statelessComponent(__MODULE__);
  let iconStyle =
    Css.(
      style([
        height(`px(20)),
        width(`px(20)),
        paddingLeft(`em(0.6)),
        verticalAlign(`bottom),
        selector("text", [fontSize(`em(0.8)), SVG.fill(`hex("989898"))]),
        selector("circle", [SVG.stroke(`hex("ccc"))]),
        cursor(`default),
      ])
    );
  let make = _children => {
    ...component,
    render: _self => {
      <svg className=iconStyle viewBox="0 0 20 20">
        <text x="10" y="14" fontWeight="bold" textAnchor="middle">
          {React.string("?")}
        </text>
        <circle cx="10" cy="10" r="8.5" strokeWidth="1" fill="none" />
      </svg>;
    },
  };
};

let make = (~cdf: FC__Types.Dist.t, _children) => {
  ...component,
  render: _self => {
    <FC__PageCard>
      <FC__PageCard.HeaderRow>
         <h4 className=Styles.cardTitle>
           "New Prediction"->React.string
           <IconQuestionmark />
         </h4>
       </FC__PageCard.HeaderRow>
      <FC__PageCard.HeaderRow>
         <div className=Css.(style([display(`flex), margin2(~v=`zero, ~h=`em(-1.5))]))>
           <FC__Tab.Flex isActive=true> "Simple"->React.string </FC__Tab.Flex>
           <FC__Tab.Flex isActive=false>
             "Free-form"->React.string
           </FC__Tab.Flex>
           <FC__Tab.Flex isActive=false> "Custom"->React.string </FC__Tab.Flex>
         </div>
       </FC__PageCard.HeaderRow>
      <div className=Styles.chartContainer>
        <FC__CdfChart__Large cdf width=None minX=2.0 maxX=12.0 />
      </div>
      <div className=Styles.inputSection>
        <input
          type_="text"
          className=Styles.textInputFullWidth
          placeholder="5 to 50"
        />
      </div>
      <div className=Styles.commentSection>
        <div className=Styles.inputHeader> "Comment"->React.string </div>
        <textarea className=Styles.textAreaFullWidth rows=5 />
        <button className=Styles.buttonFullWidth>
          "Submit Prediction"->React.string
        </button>
      </div>
    </FC__PageCard>;
  },
};
