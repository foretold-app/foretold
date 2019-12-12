[@bs.module "@foretold/components/src/Charts/cdfChart.js"]
external cdfChart: ReasonReact.reactClass = "default";

type data = {
  .
  "xs": array(float),
  "ys": array(float),
};

[@react.component]
let make =
    (
      ~width=?,
      ~height=?,
      ~verticalLine=?,
      ~showVerticalLine=?,
      ~marginBottom=?,
      ~showDistributionLines=?,
      ~maxX=?,
      ~minX=?,
      ~primaryDistribution=?,
      ~children=<Null />,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=cdfChart,
    ~props=
      makeProps(
        ~width?,
        ~height?,
        ~verticalLine?,
        ~marginBottom?,
        ~showVerticalLine?,
        ~showDistributionLines?,
        ~maxX?,
        ~minX?,
        ~primaryDistribution?,
        (),
      ),
    children,
  )
  |> ReasonReact.element;