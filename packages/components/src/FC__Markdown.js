import React from "react";
import ReactMarkdown from "react-markdown";
import MathJax from "@matejmazur/react-mathjax";
import * as RemarkMathPlugin from "remark-math";

const input = `# This is a header\n\nAnd this is a paragraph $ x=y^2 $`;

export const Markdowner = (props) => {
  const newProps = {
    ...props,
    plugins: [
      RemarkMathPlugin,
    ],
    renderers: {
      ...props.renderers,
      math: (props) =>
        <MathJax.Node>{props.value}</MathJax.Node>,
      inlineMath: (props) => <div>hihi</div>
    }
  };
  return (
    <MathJax.Context input="tex">
      <ReactMarkdown {...newProps} />
    </MathJax.Context>
  );
};

class MarkdownRender extends React.Component {
  render(){
    /* An untruthy width prop makes chart expand to container size */
      return (<div><Markdowner source={input} /></div>)
  }
}

export default MarkdownRender;