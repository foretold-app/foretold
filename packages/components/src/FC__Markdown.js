import React from "react";
import ReactMarkdown from "react-markdown";
import MathJax from "@matejmazur/react-mathjax";
import * as RemarkMathPlugin from "remark-math";

export const MarkdownRender = (props) => {
  const newProps = {
    ...props,
    plugins: [
      RemarkMathPlugin,
    ],
    renderers: {
      ...props.renderers,
      math: (props) =>
        <MathJax.Node inline>{props.value}</MathJax.Node>,
      inlineMath: (props) =>
        <MathJax.Node inline>{props.value}</MathJax.Node>,
    }
  };
  return (
    <MathJax.Context input="tex">
      <ReactMarkdown {...newProps} />
    </MathJax.Context>
  );
};

export default MarkdownRender;