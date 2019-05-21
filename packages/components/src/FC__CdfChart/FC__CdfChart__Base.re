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
    ~marginTop: int=?,
    ~maxX: float=?,
    ~minX: float=?,
    ~primaryDistribution: data=?,
    ~onHover: float => unit=?,
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
      ~marginTop=?,
      ~showDistributionLines=?,
      ~maxX=?,
      ~minX=?,
      ~onHover=?,
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
        ~marginTop?,
        ~onHover?,
        ~showVerticalLine?,
        ~showDistributionLines?,
        ~maxX?,
        ~minX?,
        ~primaryDistribution?,
        (),
      ),
    children,
  );