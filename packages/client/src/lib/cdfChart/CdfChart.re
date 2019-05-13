[@bs.module "./cdfChart.js"]
external cdfChart: ReasonReact.reactClass = "default";

type data = {
  .
  "xs": array(float),
  "ys": array(float),
};

[@bs.obj]
external makeProps:
  (
    ~width: int=?,
    ~height: int=?,
    ~verticalLine: float=?,
    ~showVerticalLine: bool=?,
    ~showDistributionLines: bool=?,
    ~marginBottom: int=?,
    ~maxX: float=?,
    ~minX: float=?,
    ~primaryDistribution: data=?,
    unit
  ) =>
  _ =
  "";
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
      children,
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
  );